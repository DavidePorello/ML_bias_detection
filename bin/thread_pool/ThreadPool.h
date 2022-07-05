#ifndef SDP_PROJECT_THREADPOOL_H
#define SDP_PROJECT_THREADPOOL_H

#include <vector>
#include <queue>
#include <thread>
#include <condition_variable>
#include <functional>
#include "Task.h"

using namespace std;

/** thread pool. Type T must implement interface Task
 * */
template<typename T>
class ThreadPool {
public:
    /**
     * generate a new thread pool. T must be a class implementing the interface Task
     * @param task_func: all threads will run this function to process the task
     * @param num_threads: number og threads to create in the pool
     * @param run_threads_detached: if true, detach threads as soon as they are created,
     *          else you need to call ThreadPool::join_threads() to join them when the tasks are finished
     */
    ThreadPool(const function<void(T)> &task_func, int num_threads, const bool run_threads_detached);

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
