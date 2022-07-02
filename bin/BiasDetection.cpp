#include <iostream>
#include "BiasDetection.h"

using namespace std;

BiasDetection::BiasDetection(const Eigen::MatrixXf &dataset, vector<double> &labels, int &attribute,
                             int &num_categories,
                             const int &num_threads) {
    this->dataset = dataset;
    this->labels = labels;
    this->attribute_index = attribute;
    this->num_categories = num_categories;
    this->num_threads = num_threads;
}

void BiasDetection::compute_bias() {

    // create thread pool
    function<void(AlternationTask &)> alternation_func = [this](AlternationTask t) -> void {
        this->train_model(t);
    };
    ThreadPool<AlternationTask> pool(alternation_func, this->num_threads);

    // train and predict on the normal dataset
    AlternationTask normal_task;
    pool.enqueue(normal_task);

    // train and predict on the alternated dataset
    for (int i = 0; i < num_categories; i++)
        for (int j = i + 1; j < num_categories; j++) {
            AlternationTask task(i, j);
            pool.enqueue(task);
        }

    // signal the pool to stop after all previous tasks are completed
    AlternationTask stop_task(true);
    pool.enqueue(stop_task);

    pool.join_threads();
}

void BiasDetection::train_model(AlternationTask &t) {
    Eigen::MatrixXf ds = this->dataset;
    Eigen::VectorXf predictions;

    // compute the alternation function
    if (t.to_be_alternated())
        ds = getAlternatedDataset(t.getCategory1(), t.getCategory2());

    //TODO train and get predictions
    cout << t.getCategory1() << ", " << t.getCategory2() << endl;
    cout << ds << endl << endl;

    promise_predictions.set_value(predictions);
}

/**
 * get a new copy of the dataset with an alternated attribute
 * */
Eigen::MatrixXf BiasDetection::getAlternatedDataset(const int &cat1, const int &cat2) const {
    // create a copy of the dataset. Eigen takes care of the dynamic allocation
    Eigen::MatrixXf d = dataset;
    auto c1 = static_cast<float>(cat1);
    auto c2 = static_cast<float>(cat2);

    // alternation
    for (int i = 0; i < d.rows(); i++) {
        // if attribute has value c1 or c2, assign it to the other. since they are floats, we can't use normal equivalence ==
        if (abs(d(i) - c1) < 0.05)
            d(i) = c2;
        else if (abs(d(i) - c2) < 0.05)
            d(i) = c1;
    }

    return d;
}





