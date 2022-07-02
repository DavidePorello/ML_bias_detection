#ifndef SDP_PROJECT_BIASDETECTION_H
#define SDP_PROJECT_BIASDETECTION_H

#include "Eigen/Core"
#include "ThreadPool.h"
#include <future>

using namespace std;

class BiasDetection {
public:
    BiasDetection(const Eigen::MatrixXf &dataset, vector<double> &labels, int &attribute, int &num_categories, const int &num_threads);

    void compute_bias();

    void train_model(AlternationTask &t);

private:
    Eigen::MatrixXf dataset;
    vector<double> labels;
    int attribute_index;
    int num_categories;
    int num_threads;
    promise<Eigen::VectorXf> promise_predictions;
    Eigen::MatrixXf getAlternatedDataset(const int &cat1, const int &cat2) const;
};


#endif //SDP_PROJECT_BIASDETECTION_H
