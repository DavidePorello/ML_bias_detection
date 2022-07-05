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

class AlternationTask : public Task {
private:
    int category1;
    int category2;
    promise<Eigen::MatrixXf> promise_database;
public:
    AlternationTask();

    explicit AlternationTask(bool stop_signal);

    AlternationTask(int category1, int category2);

    [[nodiscard]] const int &getCategory1() const;

    [[nodiscard]] const int &getCategory2() const;

    [[maybe_unused]] bool to_be_alternated() const;

    future<Eigen::MatrixXf> get_future();

    void set_alternated_dataset(Eigen::MatrixXf alt_dataset);
};

class KFoldTask : public Task {
private:
    int test_fold_index;
    Eigen::MatrixXf dataset;
    promise<Eigen::VectorXf> promise_predictions;
public:
    KFoldTask();

    explicit KFoldTask(bool stop_signal);

    KFoldTask(Eigen::MatrixXf dataset, int test_fold_index);

    [[nodiscard]] const int &get_test_fold_index() const;

    Eigen::MatrixXf &getDataset();

    future<Eigen::VectorXf> get_future();

    void set_predictions(Eigen::VectorXf preds);
};


#endif //SDP_PROJECT_TASK_H
