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
    KFold(int num_folds, int num_threads, Eigen::VectorXf labels);

    future<vector<future<Eigen::VectorXf>>>&& compute_predictions_async_pool(future<Eigen::MatrixXf> &dataset);

    vector<future<Eigen::VectorXf>> compute_predictions(const Eigen::MatrixXf &dataset);

    void set_model(ModelML *model_ml);

private:
    int num_folds;
    int num_threads;
    Eigen::VectorXf labels;
    ModelML *model;

    void run_model(Eigen::MatrixXf dataset, KFoldTask &t);
};





#endif //SDP_PROJECT_KFOLD_H
