#ifndef SDP_PROJECT_KFOLDTASK_H
#define SDP_PROJECT_KFOLDTASK_H

#include <future>
#include "../thread_pool/Task.h"
#include "../data_types.h"

using namespace std;

class KFoldTask : public Task {
private:
    int test_fold_index;
    AlternatedMatrix dataset;
    promise<Eigen::MatrixXf> promise_predictions;
public:
    KFoldTask();

    explicit KFoldTask(bool stop_signal);

    KFoldTask(AlternatedMatrix dataset, int test_fold_index);

    [[nodiscard]] const int &get_test_fold_index() const;

    [[nodiscard]] const AlternatedMatrix &getDataset() const;

    future<Eigen::MatrixXf> get_future();

    void set_predictions(Eigen::MatrixXf preds);
};

#endif //SDP_PROJECT_KFOLDTASK_H
