#include <iostream>
#include "KFold.h"

KFold::KFold(int num_folds, int num_threads, Eigen::VectorXf labels) {
    this->num_folds = num_folds;
    this->num_threads = num_threads;
    this->labels = labels;
    this->model = nullptr;
}

void KFold::set_model(ModelML *model_ml) {
    this->model = model_ml;
}

future<vector<future<Eigen::VectorXf>>>&& KFold::compute_predictions_async_pool(future<Eigen::MatrixXf> &dataset) {
    future<vector<future<Eigen::VectorXf>>> fut =  async(launch::async, [this, &dataset]() -> vector<future<Eigen::VectorXf>> {
        Eigen::Matrix d = dataset.get();
        return compute_predictions(d);
    });
    return move(fut);
}

vector<future<Eigen::VectorXf>> KFold::compute_predictions(const Eigen::MatrixXf &dataset) {
    if (this->model == nullptr) {
        cerr << "Model is not defined. Please call BiasDetection::setModel()" << endl;
        exit(1);
    }

    vector<future<Eigen::VectorXf>> predictions;

    // create thread pool
    function<void(KFoldTask &&)> kfold_func = [this, &dataset](KFoldTask &&t) -> void {
        this->run_model(dataset, t);
    };
    ThreadPool<KFoldTask> pool(kfold_func, this->num_threads, true);

    // train and predict on the alternated dataset
    for (int i = 0; i < num_folds; i++) {
        KFoldTask task(i);
        predictions.emplace_back(task.get_future());
        pool.enqueue(move(task));
    }

    // signal the pool to stop after all previous tasks are completed
    KFoldTask stop_task(true);
    pool.enqueue(move(stop_task));

    return predictions;
}

void KFold::run_model(Eigen::MatrixXf dataset, KFoldTask &t) {
    Eigen::VectorXf predictions;
    Eigen::MatrixXf test;
    int num_records = dataset.rows();
    int num_cols = dataset.cols();
    int fold_size = (num_records + num_folds - 1) / num_folds; // fold_size = ceil(num_records/num_folds)
    int test_fold_index = t.get_test_fold_index();
    int test_fold_start = test_fold_index * fold_size;

    // TODO test me

    // pop test fold from dataset
    if(test_fold_index != num_cols-1) {
        test = dataset.block(test_fold_start, 0, test_fold_start+fold_size, num_cols);
        dataset.block(test_fold_start, 0, num_records, num_cols) = dataset.block(test_fold_start+num_records, 0, num_records, num_cols);
        dataset.conservativeResize(num_records, num_cols - fold_size);
    }else{
        test = dataset.block(test_fold_start, 0, num_records, num_cols);
        dataset.conservativeResize(num_records, fold_size*(num_folds-1));
    }

    // train the model
    model->fit(dataset, labels);
    // predict the values
    model->predict(test, predictions);

    t.set_predictions(predictions);
}


