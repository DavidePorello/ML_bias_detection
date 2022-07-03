#ifndef SDP_PROJECT_KFOLD_H
#define SDP_PROJECT_KFOLD_H

#include <future>
#include <vector>
#include "Eigen/Core"
#include "ThreadPool.h"
#include "ModelML.h"

using namespace std;

class KFold {
public:
    KFold(int num_folds, int num_threads, const Eigen::VectorXf &labels, ModelML &model);

    future<vector<future<Eigen::VectorXf>>> compute_predictions_async_pool(future<Eigen::MatrixXf> &dataset);

    vector<future<Eigen::VectorXf>> compute_predictions(const Eigen::MatrixXf &dataset);

    void join_threads();
private:
    int num_folds;
    int num_threads;
    Eigen::VectorXf labels;
    ModelML& model;
    vector<unique_ptr<ThreadPool<KFoldTask>>> pools;

    void run_model(KFoldTask &t);
    [[nodiscard]] int get_fold_start_index(int num_records, int fold_index) const;
};






#endif //SDP_PROJECT_KFOLD_H
