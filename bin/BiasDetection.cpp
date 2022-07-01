#include "BiasDetection.h"

using namespace std;

BiasDetection::BiasDetection(mat &dataset, vector<double> &labels, int &attribute, int &num_categories,
                             const int &num_threads) {
    this->dataset = dataset;
    this->labels = labels;
    this->attribute = attribute;
    this->num_categories = num_categories;
    this->num_threads = num_threads;
}

void BiasDetection::compute_bias() {

    // create thread pool
    function<void(AlternationTask &)> alternation_func = [this](AlternationTask t) -> void {
        this->compute_alternation(t);
    };
    ThreadPool<AlternationTask> pool(alternation_func, this->num_threads);

    // train on the normal dataset
    AlternationTask normal_task(false);
    pool.enqueue(normal_task);

    // train on the alternated dataset
    for (int i = 0; i < num_categories; i++)
        for (int j = i + 1; j < num_categories; j++) {
            AlternationTask task(i,j);
            pool.enqueue(task);
        }

    // signal the pool to stop after all previous tasks are completed
    AlternationTask stop_task(true);
    pool.enqueue(stop_task);

}

void BiasDetection::compute_alternation(AlternationTask &t) const {

}

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



