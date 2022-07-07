#ifndef SDP_PROJECT_DATASETALTERNATOR_H
#define SDP_PROJECT_DATASETALTERNATOR_H

#include <future>
#include "Eigen/Core"
#include "../thread_pool/ThreadPool.h"
#include "AlternationTask.h"


using namespace std;

class DatasetAlternator {
public:
    DatasetAlternator(const Eigen::MatrixXf &dataset, int &attribute, int &num_categories,
                      const int &num_threads);

    vector<future<Eigen::MatrixXf>> run();

    void join_threads();
private:
    Eigen::MatrixXf dataset;
    int attribute_index;
    int num_categories{};
    int num_threads{};
    unique_ptr<ThreadPool<AlternationTask>> pool;

    void compute_alternated_dataset(AlternationTask &t);
};


#endif //SDP_PROJECT_DATASETALTERNATOR_H
