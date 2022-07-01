#include "Task.h"

AlternationTask::AlternationTask(bool stop_signal) {
    this->category1 = -1;
    this->category2 = -1;
    this->stop_signal = stop_signal;
}

AlternationTask::AlternationTask(int category1, int category2) {
    this->category1 = category1;
    this->category2 = category2;
    this->stop_signal = false;
}

bool AlternationTask::isStopTask() const {
    return stop_signal;
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