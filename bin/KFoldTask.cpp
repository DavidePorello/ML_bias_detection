#include "Task.h"

KFoldTask::KFoldTask(bool stop_signal) {
    this->test_fold_index = 0;
    this->stop_signal = stop_signal;
}

KFoldTask::KFoldTask() {
    this->test_fold_index = 0;
    this->stop_signal = false;
}

KFoldTask::KFoldTask(int test_fold_index) {
    this->test_fold_index = test_fold_index;
    this->stop_signal = false;
}

const int &KFoldTask::get_test_fold_index() const {
    return test_fold_index;
}

future<Eigen::VectorXf> KFoldTask::get_future(){
    return promise_predictions.get_future();
}

void KFoldTask::set_predictions(Eigen::VectorXf preds){
    promise_predictions.set_value(preds);
}

