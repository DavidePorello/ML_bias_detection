#include <iostream>
#include <numeric>
#include "KFold.h"

KFold::KFold(int num_folds, int num_threads,
             const Eigen::VectorXf &labels, ModelML &model) : num_folds(num_folds),
                                                              num_threads(num_threads),
                                                              labels(labels),
                                                              model(model) {}


future<vector<future<Eigen::VectorXf>>> KFold::compute_predictions_async_pool(future<Eigen::MatrixXf> &dataset) {
    future<vector<future<Eigen::VectorXf>>> fut = async(launch::async,
                                                        [this, &dataset]() -> vector<future<Eigen::VectorXf>> {
                                                            Eigen::Matrix d = dataset.get();
                                                            return compute_predictions(d);
                                                        });
    return move(fut);
}

vector<future<Eigen::VectorXf>> KFold::compute_predictions(const Eigen::MatrixXf &dataset) {

    vector<future<Eigen::VectorXf>> predictions;

    // create thread pool
    function<void(KFoldTask &&)> kfold_func = [this](KFoldTask &&t) -> void {
        this->run_model(t);
    };
    unique_ptr<ThreadPool<KFoldTask>> pool = make_unique<ThreadPool<KFoldTask>>(kfold_func, this->num_threads, false);

    // train and predict on the alternated dataset
    for (int i = 0; i < num_folds; i++) {
        KFoldTask task(dataset, i);
        predictions.emplace_back(task.get_future());
        pool->enqueue(move(task));
    }


    // signal the pool to stop after all previous tasks are completed
    KFoldTask stop_task(true);
    pool->enqueue(move(stop_task));

    // save the thread pool in the kfold object (else, it would be destroyed at the end of the func and threads wouldn't be able to access the queue anymore)
    pools.emplace_back(move(pool));

    return predictions;
}

void KFold::run_model(KFoldTask &t) {
    Eigen::VectorXf predictions, data_labels;
    Eigen::MatrixXf test, data;
    const Eigen::MatrixXf &dataset = t.getDataset();
    int num_records = dataset.rows();
    int test_fold_index = t.get_test_fold_index();
    int test_fold_start = get_fold_start_index(num_records, test_fold_index);
    int test_fold_end = get_fold_start_index(num_records, test_fold_index + 1);
    int test_fold_size = test_fold_end - test_fold_start;

    // define vector of indices of the records in the dataset and test fold
    vector<int> data_indices(num_records - test_fold_size);
    iota(data_indices.begin(), data_indices.begin() + test_fold_start, 0);
    iota(data_indices.begin() + test_fold_start, data_indices.end(), test_fold_end);
    vector<int> test_indices(test_fold_end - test_fold_start);
    iota(test_indices.begin(), test_indices.end(), test_fold_start);

    // pop test fold from dataset
    data = dataset(data_indices, Eigen::all);
    test = dataset(test_indices, Eigen::all);
    data_labels = labels(data_indices);

    // train the model
    model.fit(data, data_labels);
    // predict the values
    model.predict(test, predictions);

    t.set_predictions(predictions);
}

int KFold::get_fold_start_index(int num_records, int fold_index) const {
    return fold_index * (num_records / num_folds) + min(fold_index, num_records % num_folds);
}

/**
 * Join all threads spawned by this object
 */
void KFold::join_threads() {
    for (unique_ptr<ThreadPool<KFoldTask>> &pool : pools) {
        // signal the pool to stop (shouldn't be needed, just to make sure)
        KFoldTask stop_task(true);
        pool->enqueue(move(stop_task));

        //join threads
        pool->join_threads();
    }
}

