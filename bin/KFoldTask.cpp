#include "Task.h"

KFoldTask::KFoldTask(bool stop_signal) : Task(stop_signal) {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask() : Task() {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask(Eigen::MatrixXf dataset, int test_fold_index) : Task() {
    this->dataset = move(dataset);
    this->test_fold_index = test_fold_index;
}

const int &KFoldTask::get_test_fold_index() const {
    return test_fold_index;
}

Eigen::MatrixXf &KFoldTask::getDataset(){
    return ref(dataset);
}

future<Eigen::VectorXf> KFoldTask::get_future() {
    return promise_predictions.get_future();
}

void KFoldTask::set_predictions(Eigen::VectorXf preds) {
    promise_predictions.set_value(preds);
}

