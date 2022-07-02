#ifndef SDP_PROJECT_TASK_H
#define SDP_PROJECT_TASK_H

#include "Eigen/Core"
#include <future>

using namespace std;

class Task {
protected:
    bool stop_signal;
public:
    [[nodiscard]] bool isStopTask() const { return stop_signal; };
};

class AlternationTask : public Task {
private:
    int category1;
    int category2;
    promise<Eigen::MatrixXf> promise_database;
public:
    AlternationTask();

    explicit AlternationTask(bool stop_signal);

    AlternationTask(int category1, int category2);

    const int &getCategory1() const;

    const int &getCategory2() const;

    bool to_be_alternated() const;

    future<Eigen::MatrixXf> get_future();

    void set_alternated_dataset(Eigen::MatrixXf alt_dataset);
};

class KFoldTask : public Task {
private:
    int test_fold_index;
    promise<Eigen::VectorXf> promise_predictions;
public:
    KFoldTask();

    explicit KFoldTask(bool stop_signal);

    KFoldTask(int test_fold_index);

    const int &get_test_fold_index() const;

    future<Eigen::VectorXf> get_future();

    void set_predictions(Eigen::VectorXf preds);
};


#endif //SDP_PROJECT_TASK_H
