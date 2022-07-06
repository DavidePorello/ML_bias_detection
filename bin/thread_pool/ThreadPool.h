#ifndef SDP_PROJECT_THREADPOOL_H
#define SDP_PROJECT_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>
#include "Task.h"

using namespace std;

/**
 * Generic thread pool. Jobs are defined by the task class T. The worker threads run a constant function f(T)
 * @tparam T: class of the tasks to be processed by the thread pool. it must implement interface Task
 */
template<typename T>
class ThreadPool {
public:
    /**
     * generate a new thread pool.
     * @param task_func: all threads will run this function to process the task
     * @param num_threads: number of threads to create in the pool
     * @param run_threads_detached: if true, detach threads as soon as they are created,
     *          else you need to call ThreadPool::join_threads() to join them when the tasks are finished
     * @tparam T: class of the tasks to be processed by the thread pool. it must implement interface Task
     */
    ThreadPool(const function<void(T)> &task_func, int num_threads, bool run_threads_detached=false);

    /**
     * Put a new task in queue
     * @param t: a single object containing all the parameter required to process a task
     */
    void enqueue(T t);

    /**
     * Blocking function that joins all the threads in the pool
     */
    void join_threads();

private:
    function<void(T)> task_func;
    queue<T> jobs;
    mutex m;
    condition_variable cv;
    vector<thread> threads;
    bool terminate;

    void thread_func();
};


#endif //SDP_PROJECT_THREADPOOL_H
