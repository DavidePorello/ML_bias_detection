#ifndef SDP_PROJECT_BIASDETECTION_H
#define SDP_PROJECT_BIASDETECTION_H

#include <future>
#include "Eigen/Core"
#include "ThreadPool.h"


using namespace std;

class BiasDetection {
public:
    BiasDetection(const Eigen::MatrixXf &dataset, int &attribute, int &num_categories,
                  const int &num_threads);

    vector<future<Eigen::MatrixXf>> compute_bias(const int &parallelization_mode);

    void join_threads();
private:
    Eigen::MatrixXf dataset;
    int attribute_index;
    int num_categories{};
    int num_threads{};
    unique_ptr<ThreadPool<AlternationTask>> pool;

    void compute_alternated_dataset(AlternationTask &t);
};


#endif //SDP_PROJECT_BIASDETECTION_H
