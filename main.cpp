#include <iostream>
#include <memory>

#include "Eigen/core"

#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"
#include "bin/dataset_alternator/DatasetAlternator.h"
#include "bin/kfold/KFold.h"
#include "bin/ml/ModelML.h"
#include "bin/ml/LinearRegression.h"
#include "bin/ml/PolynomialRegression.h"

/////////// CONFIG

#define NUM_THREADS_ALTERNATION 8
#define NUM_THREADS_KFOLD 10
#define NUM_FOLDS 10

/** Indicate which PBA attribute to test */
char attribute[20] = "gender";
/** How many different categorical values can the PBA have? */ //TODO possiamo prenderlo direttamente dal dataset?
int attribute_num_categories = 2;

// TODO implement these options
/** Do we parallelize the computation of the alternation function?
 * @var 0 -> no parallelization (compute sequentially)
 * @var 1 -> use a thread pool
 * @var 2 -> use 1 async function for each alternation function
 * */
int alternation_parallelization_mode = 0;

/** Define which model to use by uncommenting the desired model */
//LinearRegression model;
PolynomialRegression model(2);
////////////////////////

using namespace std;

double kl_divergence(const Eigen::VectorXf &p, const Eigen::VectorXf &q, bool unbiased);

int main() {
    CleanedDataset d;
    const Eigen::MatrixXf m = d.getDataset();
    vector<string> classes = d.getClasses();
    Eigen::VectorXf labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);

    // Initialize worker classes
    DatasetAlternator bd(m, attr_index, attribute_num_categories, NUM_THREADS_ALTERNATION);
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels, static_cast<ModelML &> (model));


    // compute the alternated datasets
    vector<future<Eigen::MatrixXf>> alt_datasets = bd.run(alternation_parallelization_mode);

    // train the model using kfold on the standard model and get the true predictions.
    vector<future<Eigen::VectorXf>> standard_predictions = kfold.compute_predictions(m);

    // train and predict on the alternated datasets
    vector<future<vector<future<Eigen::VectorXf>>>> alt_predictions;
    for (future<Eigen::MatrixXf> &data : alt_datasets)
        alt_predictions.emplace_back(move(kfold.compute_predictions_async_pool(data)));

    // compute KL and produce plots
    for(future<Eigen::VectorXf>& p: standard_predictions)
        cout << p.get();
    for(future<vector<future<Eigen::VectorXf>>>& v : alt_predictions){
        for(future<Eigen::VectorXf>& p: v.get())
            cout << p.get();
    }


    // ensure all spawned threads terminated before destroying their pools
    bd.join_threads();
    kfold.join_threads();

    return 0;
}

/** Utility function to compute the standard deviation in place.
 *  @param p the array we want to compute the standard deviation
 *  @param mean the mean of p
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 *  */
double stddev(const Eigen::VectorXf &p, double mean, bool unbiased=false) {
    float diff;
    float sum = 0;
    for(float i : p) {
        // compute difference
        diff = i - mean;
        sum += diff*diff;
    }
    if (unbiased) {
        // divide by N - 1
        sum /= p.size()-1;
    } else {
        // divide by N
        sum /= p.size();
    }
    // return square root
    return std::sqrt(sum);
}

/** Computes the KL divergence (p||q)
 *  @param p is the array of predicted values
 *  @param q is the array of alternative predicted values
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 */
double kl_divergence(const Eigen::VectorXf &p, const Eigen::VectorXf &q, bool unbiased) {
    // compute mean and standard deviation
    double mu_1 = p.mean();
    double sigma_1 = stddev(p, mu_1, unbiased);
    double mu_2 = q.mean();
    double sigma_2 = stddev(q, mu_2, unbiased);
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + ((sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2)) - 0.5;
    return kl;
}
