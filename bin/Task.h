#ifndef SDP_PROJECT_TASK_H
#define SDP_PROJECT_TASK_H

#include "Eigen/Core"
#include <future>

using namespace std;

class Task {
public:
    [[nodiscard]] virtual bool isStopTask() const = 0;
};

class AlternationTask : public Task {
private:
    int category1;
    int category2;
    bool stop_signal;
    promise<Eigen::VectorXf> promise_predictions;
public:
    AlternationTask();
    explicit AlternationTask(bool stop_signal);

    AlternationTask(int category1, int category2);

    [[nodiscard]] bool isStopTask() const override;

    const int &getCategory1() const;
    const int &getCategory2() const;
    bool to_be_alternated() const;
    future<Eigen::VectorXf> get_future();
    void set_predictions(Eigen::VectorXf &preds);
};




#endif //SDP_PROJECT_TASK_H
