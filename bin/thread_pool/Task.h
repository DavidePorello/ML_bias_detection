#ifndef SDP_PROJECT_TASK_H
#define SDP_PROJECT_TASK_H

#include "Eigen/Core"
#include <future>

using namespace std;

class Task {
protected:
    bool stop_signal;

public:
    Task() : stop_signal(false) {};

    explicit Task(bool stop_signal) : stop_signal(stop_signal) {};

    [[nodiscard]] bool isStopTask() const { return stop_signal; };
};


#endif //SDP_PROJECT_TASK_H
