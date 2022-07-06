#include <iostream>
#include "ThreadPool.h"
#include "../dataset_alternator/AlternationTask.h"
#include "../kfold/KFoldTask.h"

template<typename T>
ThreadPool<T>::ThreadPool(const function<void(T)> &task_func, int num_threads, const bool run_threads_detached) {
    this->task_func = task_func;
    terminate = false;
    int i;
    assert(num_threads > 0);
    for (i = 0; i < num_threads; i++) {
        threads.emplace_back(thread([this]() -> void { thread_func(); }));
        if (run_threads_detached)
            threads.back().detach();
    }
}

template<typename T>
void ThreadPool<T>::enqueue(T t) {
    unique_lock lock{m};
    jobs.push(move(t));
    cv.notify_one();
}

template<typename T>
void ThreadPool<T>::thread_func() {
    T t;
    while (true) {
        {
            unique_lock lock{m};
            while (jobs.empty() && !terminate)
                cv.wait(lock);
            if (terminate)
                return;
            t = move(jobs.front());
            jobs.pop();
            // if the task is a termination task, signal other threads and exit
            if (t.isStopTask()) {
                terminate = true;
                cv.notify_all();
                return;
            }
        }
        this->task_func(move(t));
    }
}

template<typename T>
void ThreadPool<T>::join_threads() {
    for (auto &t : threads)
        t.join();
}

template class ThreadPool<AlternationTask>;
template class ThreadPool<KFoldTask>;
