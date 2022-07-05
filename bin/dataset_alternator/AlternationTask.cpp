#include "AlternationTask.h"

AlternationTask::AlternationTask(bool stop_signal) :Task(stop_signal){
    this->category1 = -1;
    this->category2 = -1;
}

AlternationTask::AlternationTask():Task() {
    this->category1 = -1;
    this->category2 = -1;
}

AlternationTask::AlternationTask(int category1, int category2) :Task(){
    this->category1 = category1;
    this->category2 = category2;
}

const int &AlternationTask::getCategory1() const {
    return category1;
}

const int &AlternationTask::getCategory2() const {
    return category2;
}

bool AlternationTask::to_be_alternated() const {
    return (category1 > 0 && category2 > 0);
}

future<AlternatedDataset> AlternationTask::get_future(){
    return promise_database.get_future();
}

void AlternationTask::set_alternated_dataset(AlternatedDataset alt_dataset){
    promise_database.set_value(alt_dataset);
}

