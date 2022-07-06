#ifndef SDP_PROJECT_ALTERNATIONTASK_H
#define SDP_PROJECT_ALTERNATIONTASK_H

#include "Eigen/Core"
#include <future>
#include "../thread_pool/Task.h"

using namespace std;

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

#endif //SDP_PROJECT_ALTERNATIONTASK_H
