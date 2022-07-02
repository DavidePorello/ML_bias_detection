#include <iostream>
#include <memory>

#include "Eigen/core"

#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"
#include "bin/BiasDetection.h"
#include "bin/KFold.h"
#include "bin/LinearRegression.h"

/////////// CONFIG

#define NUM_THREADS_ALTERNATION 8
#define NUM_THREADS_KFOLD 10
#define NUM_FOLDS 10
char attribute[20] = "gender";
int attribute_num_categories = 2;
bool use_linear_regression = true;

// TODO implement these options
/**Do we parallelize the computation of the alternation function?
 * @var 0 -> no parallelization (compute sequentially)
 * @var 1 -> use a thread pool
 * @var 2 -> use 1 async function for each alternation function
 * */
int alternation_parallelization_mode = 0;

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
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels);
    // TODO add polynomial regression
    if (use_linear_regression) {
        LinearRegression model;
        kfold.set_model(&model);
    }

    // compute the alternated datasets
    vector<future<Eigen::MatrixXf>> alt_datasets = bd.compute_bias(alternation_parallelization_mode);

    // train the model using kfold on the standard model and get the true predictions.
    vector<future<Eigen::VectorXf>> standard_predictions = kfold.compute_predictions(m);

    // train and predict on the alternated datasets
    vector<future<vector<future<Eigen::VectorXf>>>> alt_predictions;
    for (future<Eigen::MatrixXf> &data : alt_datasets)
        alt_predictions.emplace_back(kfold.compute_predictions_async_pool(data));

    cout << thread::hardware_concurrency() << endl;

    cout << standard_predictions[0].get() << endl;

    return 0;
}

template<typename F>
future<F> &&ready_future(F f) {
    promise<F> p;
    p.set_value(f);
    return move(p.get_future());
}

