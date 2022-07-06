#include <iostream>
#include <memory>

#include "Eigen/Core"

#include "dataset/CleanedDataset.h"
#include "bin/dataset_alternator/DatasetAlternator.h"
#include "bin/kfold/KFold.h"
#include "bin/ml/LinearRegression.h"
#include "bin/ml/PolynomialRegression.h"
#include "bin/plot/PlotML.h"
#include "bin/post_process.h"

/////////// CONFIG

#define NUM_THREADS_ALTERNATION 8
#define NUM_THREADS_KFOLD 8
#define NUM_FOLDS 10

/** Indicate which PBA attribute to test */
const string attribute = "race";
const string label_name = "wage";
/** How many different categorical values can the PBA have? */ //TODO possiamo prenderlo direttamente dal dataset?
int attribute_num_categories = 5;

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

int main() {
    CleanedDataset d;
    const Eigen::MatrixXf m = d.getDataset();
    vector<string> classes = d.getClasses();
    Eigen::VectorXf labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);

    // Initialize worker classes
    DatasetAlternator bd(m, attr_index, attribute_num_categories, NUM_THREADS_ALTERNATION);
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels, static_cast<ModelML &> (model), attribute_num_categories, attr_index);
    PlotML plotter(NUM_FOLDS);

    // compute the alternated datasets
    vector<future<Eigen::MatrixXf>> alt_datasets = bd.run(alternation_parallelization_mode);

    // train the model using kfold on the standard model and get the true predictions.
    vector<future<Eigen::MatrixXf>> standard_predictions_f = kfold.compute_predictions(m);

    // train and predict on the alternated datasets
    vector<future<vector<future<Eigen::MatrixXf>>>> alt_predictions_f;
    for (future<Eigen::MatrixXf> &data: alt_datasets)
        alt_predictions_f.emplace_back(move(kfold.compute_predictions_async_pool(data)));

    ////////////////////////////////////
    /// collect all results (go out of parallelization) and post process
    ////////////////////////////////////

    //collect results of predictions on the true dataset
    Eigen::MatrixXf true_means(attribute_num_categories, NUM_FOLDS); // matrix of prediction means for each category and fold
    Eigen::MatrixXf true_stddevs(attribute_num_categories, NUM_FOLDS); // matrix of prediction standard deviations for each category and fold
    for (int i=0; i<standard_predictions_f.size(); i++) {
        Eigen::MatrixXf pred = standard_predictions_f[i].get();
        true_means.col(i) = pred.col(0);
        true_stddevs.col(i) = pred.col(1);
    }

    // collect results of alternated predictions
    vector<Eigen::MatrixXf> alt_means; // each pred is a vector, containing the average of the predictions over each category and fold
    vector<Eigen::MatrixXf> alt_stddevs; // each pred is a vector, containing the standard deviation of the predictions over each category and fold
    for (future<vector<future<Eigen::MatrixXf>>> &p: alt_predictions_f) {
        Eigen::MatrixXf cur_means(attribute_num_categories, NUM_FOLDS);
        Eigen::MatrixXf cur_stddevs(attribute_num_categories, NUM_FOLDS);
        int i = 0;
        for (future<Eigen::MatrixXf> &p2: p.get()) {
            Eigen::MatrixXf pred = p2.get();
            cur_means.col(i) = pred.col(0);
            cur_stddevs.col(i++) = pred.col(0);
        }
        alt_means.emplace_back(cur_means);
        alt_stddevs.emplace_back(cur_stddevs);
    }

    // print plots
    process_results(d, plotter, true_means, true_stddevs, alt_means, alt_stddevs, label_name);

    // ensure all spawned threads terminated before destroying their pools
    bd.join_threads();
    kfold.join_threads();

    return 0;
}


