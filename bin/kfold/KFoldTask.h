#ifndef SDP_PROJECT_KFOLDTASK_H
#define SDP_PROJECT_KFOLDTASK_H

#include <future>
#include "../thread_pool/Task.h"

using namespace std;

class KFoldTask : public Task {
private:
    int test_fold_index;
    Eigen::MatrixXf dataset;
    promise<Eigen::MatrixXf> promise_predictions;
public:
    KFoldTask();

    explicit KFoldTask(bool stop_signal);

    KFoldTask(Eigen::MatrixXf dataset, int test_fold_index);

    [[nodiscard]] const int &get_test_fold_index() const;

    Eigen::MatrixXf &getDataset();

    future<Eigen::MatrixXf> get_future();

    void set_predictions(Eigen::MatrixXf preds);
};

#endif //SDP_PROJECT_KFOLDTASK_H
