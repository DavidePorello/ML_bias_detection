#include <iostream>
#include <memory>

#include "Eigen/core"

#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"
#include "bin/BiasDetection.h"
#include "bin/KFold.h"
#include "bin/ModelML.h"
#include "bin/LinearRegression.h"

/////////// CONFIG

#define NUM_THREADS_ALTERNATION 8
#define NUM_THREADS_KFOLD 10
#define NUM_FOLDS 10
char attribute[20] = "gender";
int attribute_num_categories = 2;

// TODO implement these options
/** Do we parallelize the computation of the alternation function?
 * @var 0 -> no parallelization (compute sequentially)
 * @var 1 -> use a thread pool
 * @var 2 -> use 1 async function for each alternation function
 * */
int alternation_parallelization_mode = 0;

/** Define which model to use by uncommenting the desired model
 * @var LinearRegression
 * @var TODO complete with polynomial
 * */
LinearRegression model;
// PolynomialRegression model;
////////////////////////

using namespace std;

template<typename F>
future<F> &&ready_future(F f);

int main() {
    CleanedDataset d;
    const Eigen::MatrixXf m = d.getDataset();
    vector<string> classes = d.getClasses();
    Eigen::VectorXf labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);

    BiasDetection bd(m, attr_index, attribute_num_categories, NUM_THREADS_ALTERNATION);
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels, static_cast<ModelML &> (model));


    // compute the alternated datasets
    vector<future<Eigen::MatrixXf>> alt_datasets = bd.compute_bias(alternation_parallelization_mode);

    // train the model using kfold on the standard model and get the true predictions.
    vector<future<Eigen::VectorXf>> standard_predictions = kfold.compute_predictions(m);

    // train and predict on the alternated datasets
    vector<future<vector<future<Eigen::VectorXf>>>> alt_predictions;
    for (future<Eigen::MatrixXf> &data : alt_datasets)
        alt_predictions.emplace_back(move(kfold.compute_predictions_async_pool(data)));

    // TODO temp code to ensure results are not optimized and threads return. delete
    for(future<vector<future<Eigen::VectorXf>>>& v : alt_predictions){
        for(future<Eigen::VectorXf>& p: v.get())
            cout << p.get();
    }

    // ensure all spawned threads terminated before destroying their pools
    bd.join_threads();
    kfold.join_threads();

    return 0;
}

template<typename F>
future<F> &&ready_future(F f) {
    promise<F> p;
    p.set_value(f);
    return move(p.get_future());
}

