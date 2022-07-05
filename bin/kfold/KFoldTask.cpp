#include "KFoldTask.h"

KFoldTask::KFoldTask(bool stop_signal) : Task(stop_signal) {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask() : Task() {
    this->test_fold_index = 0;
}

KFoldTask::KFoldTask(AlternatedDataset dataset, int test_fold_index) : Task() {
    this->dataset = move(dataset);
    this->test_fold_index = test_fold_index;
}

const int &KFoldTask::get_test_fold_index() const {
    return test_fold_index;
}

AlternatedDataset &KFoldTask::getDataset(){
    return ref(dataset);
}

future<vector<float>> KFoldTask::get_future() {
    return promise_predictions.get_future();
}

void KFoldTask::set_predictions(vector<float> preds) {
    promise_predictions.set_value(move(preds));
}

