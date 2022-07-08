#include "KFoldTask.h"

KFoldTask::KFoldTask(bool stop_signal) : Task(stop_signal) {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask() : Task() {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask(AlternatedMatrix dataset, int test_fold_index) : Task() {
    this->dataset = move(dataset);
    this->test_fold_index = test_fold_index;
}

const int &KFoldTask::get_test_fold_index() const {
    return test_fold_index;
}

const AlternatedMatrix &KFoldTask::getDataset() const{
    return ref(dataset);
}

future<Eigen::MatrixXf> KFoldTask::get_future() {
    return promise_predictions.get_future();
}

void KFoldTask::set_predictions(Eigen::MatrixXf preds) {
    promise_predictions.set_value(move(preds));
}

