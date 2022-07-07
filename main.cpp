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

#define NUM_THREADS_KFOLD 8
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
    const Eigen::MatrixXf m = d.getDataset();
    vector<string> classes = d.getAttributes();
    Eigen::VectorXf labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);
    int attribute_num_categories = d.getNumberOfValues(attr_index);

    // TODO remove dataset shrinker
    /*
    m.conservativeResize(1000, 10);
    labels.conservativeResize(1000);
     */

    // Initialize worker classes
    KFold kfold(NUM_FOLDS, NUM_THREADS_KFOLD, labels, static_cast<ModelML &> (model), attribute_num_categories,
                attr_index);
    PlotML plotter(NUM_FOLDS);

    cout << "Starting parallel computations..." << endl;

    // compute the alternated datasets
    //vector<future<AlternatedMatrix>> alt_datasets = bd.run();

    // train the model using kfold on the standard model and get the true predictions.
    AlternatedMatrix dataset(m, -1, -1);
    vector<future<Eigen::MatrixXf>> standard_predictions_f = kfold.compute_predictions(dataset);

    // train and predict on the alternated datasets
    vector<vector<future<Eigen::MatrixXf>>> alt_predictions_f;
    for (int i = 0; i < attribute_num_categories; i++)
        for (int j = i + 1; j < attribute_num_categories; j++) {
        AlternatedMatrix data(m, i, j);
        alt_predictions_f.emplace_back(kfold.compute_predictions(data));
    }

    ////////////////////////////////////
    /// collect all results (go out of parallelization) and post process
    ////////////////////////////////////

    // collect results of predictions on the true dataset
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
    for (vector<future<Eigen::MatrixXf>> &p: alt_predictions_f) {
        Eigen::MatrixXf cur_means(attribute_num_categories, NUM_FOLDS);
        Eigen::MatrixXf cur_stddevs(attribute_num_categories, NUM_FOLDS);
        int i = 0;
        for (future<Eigen::MatrixXf> &p2: p) {
            Eigen::MatrixXf pred = p2.get();
            cur_means.col(i) = pred.col(0);
            cur_stddevs.col(i++) = pred.col(1);
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
    kfold.join_threads();

    return 0;
}


