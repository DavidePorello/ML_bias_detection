#include <iostream>
#include "DatasetAlternator.h"

using namespace std;

/**
 * Handles the alternation function cuncurrently, using num_threads inside a thread pool
 * @param dataset: the dataset on which to run the ML model. Each row is a record, each column an attribute
 * @param attribute: index of the attribute (PBA) to alternate
 * @param num_categories: number of categories of the alternated attribute (e.g. attribute gender has 2 categories -> male, female)
 * @param num_threads: number of threads to use for the parallel computation
 */
DatasetAlternator::DatasetAlternator(const Eigen::MatrixXf &dataset, int &attribute,
                                     int &num_categories,
                                     const int &num_threads) : dataset(dataset),
                                                       attribute_index(attribute),
                                                       num_categories(num_categories),
                                                       num_threads(num_threads) {

    // create thread pool
    function<void(AlternationTask &&)> alternation_func = [this](AlternationTask &&t) -> void {
        this->compute_alternated_dataset(t);
    };
    unique_ptr<ThreadPool<AlternationTask>> p = make_unique<ThreadPool<AlternationTask>>(alternation_func,
                                                                                         this->num_threads, false);
    pool = move(p);
}

/**
 * Cuncurrently generate all alternated datasets
 * @return a vector of futures, each containing an alternated dataset
 */
vector<future<AlternatedMatrix>> DatasetAlternator::run() {

    vector<future<AlternatedMatrix>> alt_dataset_futures;

    // for each alternated dataset to compute, define a task and insert it in the thread pool
    for (int i = 0; i < num_categories; i++)
        for (int j = i + 1; j < num_categories; j++) {
            AlternationTask alternated_task(i,j);
            alt_dataset_futures.emplace_back(move(alternated_task.get_future()));
            pool->enqueue(move(alternated_task));
        }

    // signal the pool to stop after all previous tasks are completed
    AlternationTask stop_task(true);
    pool->enqueue(move(stop_task));

    return alt_dataset_futures;
}

/**
 * Given a task, compute the alternation function and return the alternated dataset through t.promise_database
 * @param t
 */
void DatasetAlternator::compute_alternated_dataset(AlternationTask &t) {
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

    AlternatedMatrix result(move(d), t.getCategory1(), t.getCategory2());
    t.set_alternated_dataset(move(result));
}

/**
 * Join all threads spawned by this object
 */
void DatasetAlternator::join_threads() {
    // signal the pool to stop (shouldn't be needed, just to make sure)
    AlternationTask stop_task(true);
    pool->enqueue(move(stop_task));

    //join threads
    pool->join_threads();
}





