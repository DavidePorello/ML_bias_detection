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

#define NUM_THREADS_ALTERNATION 1
#define NUM_THREADS_KFOLD 1
#define NUM_FOLDS 10

/** Indicate which PBA attribute to test (i.e. "sex" or "race")*/
const string attribute = "sex";

/** Display name for the prediction values */
const string label_name = "wage";

/** Define which model to use by uncommenting the desired model */
//LinearRegression model;
PolynomialRegression model(2);
////////////////////////

using namespace std;

int main() {

    cout << "Loading dataset..." << endl;
    CleanedDataset d;
    Eigen::MatrixXf m = d.getDataset();
    cout << m.rows() << endl;
    vector<string> classes = d.getAttributes();
    Eigen::VectorXf labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);
    int attribute_num_categories = d.getNumberOfValues(attr_index);

    // TODO remove dataset shrinker
    //m.conservativeResize(50, 10);
    //labels.conservativeResize(50);

    //cout << m<<endl;
    //cout << labels<<endl;

    //cout << labels << endl; // TODO remove
    cout << endl << endl << endl;

    // Initialize worker classes
    DatasetAlternator bd(m, attr_index, attribute_num_categories, NUM_THREADS_ALTERNATION);
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels, static_cast<ModelML &> (model), attribute_num_categories,
                attr_index);
    PlotML plotter(NUM_FOLDS);

    cout << "Starting parallel computations..." << endl;

    // compute the alternated datasets
    vector<future<Eigen::MatrixXf>> alt_datasets = bd.run();

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
    Eigen::MatrixXf true_means(attribute_num_categories,
                               NUM_FOLDS); // matrix of prediction means for each category and fold
    Eigen::MatrixXf true_stddevs(attribute_num_categories,
                                 NUM_FOLDS); // matrix of prediction standard deviations for each category and fold
    for (int i = 0; i < standard_predictions_f.size(); i++) {
        Eigen::MatrixXf pred = standard_predictions_f[i].get();
        true_means.col(i) = pred.col(0);
        true_stddevs.col(i) = pred.col(1);
    }
    cout << "Collected standard prediction" << endl << endl;

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
        cout << "\rCollected alternated prediction " << alt_means.size() << "/" << alt_predictions_f.size();
    }
    for(auto& i:alt_means) cout <<endl<< i << endl; // TODO remove
    for(auto& i:alt_stddevs) cout <<endl<< i << endl; // TODO remove

    // print plots
    cout << endl << "Printing plots..." << endl;
    process_results(d, plotter, true_means, true_stddevs, alt_means, alt_stddevs, label_name, attr_index);

    // ensure all spawned threads terminated before destroying their pools
    bd.join_threads();
    kfold.join_threads();

    return 0;
}


