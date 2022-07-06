#ifndef SDP_PROJECT_KFOLD_H
#define SDP_PROJECT_KFOLD_H

#include <future>
#include <vector>
#include "Eigen/Core"
#include "../thread_pool/ThreadPool.h"
#include "../ml/ModelML.h"
#include "KFoldTask.h"

using namespace std;

class KFold {
public:
    KFold(int num_folds, int num_threads, const Eigen::VectorXf &labels, ModelML &model, int num_categories,
          int attribute_index);

    future<vector<future<Eigen::MatrixXf>>> compute_predictions_async_pool(future<Eigen::MatrixXf> &dataset);

    vector<future<Eigen::MatrixXf>> compute_predictions(const Eigen::MatrixXf &dataset);

    void join_threads();

private:
    int num_folds;
    int num_threads;
    Eigen::VectorXf labels;
    ModelML &model;
    int attribute_index;
    int num_categories;
    unique_ptr<ThreadPool<KFoldTask>> pool;

    void run_model(KFoldTask &t);

    [[nodiscard]] int get_fold_start_index(int num_records, int fold_index) const;

    Eigen::MatrixXf process_results(const Eigen::MatrixXf &test, const Eigen::VectorXf &predictions, int test_fold_size) const;
};

float stddev(const Eigen::VectorXf &p, float mean, bool unbiased=false);

#endif //SDP_PROJECT_KFOLD_H
