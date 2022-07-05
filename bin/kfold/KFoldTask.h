#ifndef SDP_PROJECT_KFOLDTASK_H
#define SDP_PROJECT_KFOLDTASK_H

#include <future>
#include "../thread_pool/Task.h"
#include "../data_types.h"

using namespace std;

class KFoldTask : public Task {
private:
    int test_fold_index;
    AlternatedDataset dataset;
    promise<vector<float>> promise_predictions;
public:
    KFoldTask();

    explicit KFoldTask(bool stop_signal);

    KFoldTask(AlternatedDataset dataset, int test_fold_index);

    [[nodiscard]] const int &get_test_fold_index() const;

    AlternatedDataset &getDataset();

    future<vector<float>> get_future();

    void set_predictions(vector<float> preds);
};

#endif //SDP_PROJECT_KFOLDTASK_H
