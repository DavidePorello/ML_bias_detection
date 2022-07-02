#include <iostream>
#include "BiasDetection.h"

using namespace std;

BiasDetection::BiasDetection(const Eigen::MatrixXf &dataset, int &attribute,
                             int &num_categories,
                             const int &num_threads) {
    this->dataset = dataset;
    this->attribute_index = attribute;
    this->num_categories = num_categories;
    this->num_threads = num_threads;
}

vector<future<Eigen::MatrixXf>> BiasDetection::compute_bias(const int &parallelization_mode) {

    vector<future<Eigen::MatrixXf>> alt_dataset_futures;

    // create thread pool
    function<void(AlternationTask &&)> alternation_func = [this](AlternationTask &&t) -> void {
        this->compute_alternated_dataset(t);
    };
    ThreadPool<AlternationTask> pool(alternation_func, this->num_threads, true);

    // train and predict on the alternated dataset
    for (int i = 0; i < num_categories; i++)
        for (int j = i + 1; j < num_categories; j++) {
            AlternationTask alternated_task(i, j);
            alt_dataset_futures.emplace_back(alternated_task.get_future());
            pool.enqueue(move(alternated_task));
        }

    // signal the pool to stop after all previous tasks are completed
    AlternationTask stop_task(true);
    pool.enqueue(move(stop_task));

    return alt_dataset_futures;
}


void BiasDetection::compute_alternated_dataset(AlternationTask &t) {
    Eigen::MatrixXf d = dataset;
    auto c1 = static_cast<float>(t.getCategory1());
    auto c2 = static_cast<float>(t.getCategory2());

    // alternation
    for (int i = 0; i < d.rows(); i++) {
        // if attribute has value c1 or c2, assign it to the other. since they are floats, we can't use normal equivalence ==
        if (abs(d(i, attribute_index) - c1) < 0.05)
            d(i, attribute_index) = c2;
        else if (abs(d(i, attribute_index) - c2) < 0.05)
            d(i, attribute_index) = c1;
    }

    t.set_alternated_dataset(d);
}






