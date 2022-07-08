#ifndef SDP_PROJECT_KFOLD_H
#define SDP_PROJECT_KFOLD_H

#include <future>
#include <vector>
#include "Eigen/Core"
#include "../thread_pool/ThreadPool.h"
#include "../ml/LinearRegression.h"
#include "../ml/PolynomialRegression.h"
#include "KFoldTask.h"
#include "../data_types.h"

using namespace std;

class KFold {
public:
    KFold(int num_folds, int num_threads, const Eigen::VectorXf &labels, int modelML_type, int num_categories,
          int attribute_index);

    future<vector<future<Eigen::MatrixXf>>> compute_predictions_async_pool(future<AlternatedMatrix> &dataset);

    vector<future<Eigen::MatrixXf>> compute_predictions(const AlternatedMatrix &dataset);

    void join_threads();

private:
    int num_folds;
    int num_threads;
    Eigen::VectorXf labels;
    int modelML_type;
    int attribute_index;
    int num_categories;
    unique_ptr<ThreadPool<KFoldTask>> pool;
    mutex m; // TODO remove

    void run_model(KFoldTask &t);

    [[nodiscard]] int get_fold_start_index(int num_records, int fold_index) const;

    [[nodiscard]] Eigen::MatrixXf process_results(const Eigen::MatrixXf &test, const Eigen::VectorXf &predictions, int a1, int a2) const;

    unique_ptr<ModelML> createModel() const;
};

float stddev(const Eigen::VectorXf &p, float mean, bool unbiased=false);

#endif //SDP_PROJECT_KFOLD_H
