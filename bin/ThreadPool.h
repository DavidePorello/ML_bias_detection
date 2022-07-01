#ifndef SDP_PROJECT_THREADPOOL_H
#define SDP_PROJECT_THREADPOOL_H

#include <armadillo>
#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>

using namespace arma;
using namespace std;

class Task {
public:
    [[nodiscard]] virtual bool isStopTask() const = 0;
};

// thread pool. Type T must implement interface Task
template<typename T>
class ThreadPool {
public:
    ThreadPool(function<void(T &)> &task_func, int num_threads);

    void enqueue(T t);

private:
    const function<void(T)> task_func;
    queue<T> jobs;
    mutex m;
    condition_variable cv;
    vector<thread> threads;
    bool terminate;

    void thread_func();

};


#endif //SDP_PROJECT_THREADPOOL_H
