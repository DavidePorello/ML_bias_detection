#ifndef SDP_PROJECT_BIASDETECTION_H
#define SDP_PROJECT_BIASDETECTION_H

#include "Eigen/core"
#include "ThreadPool.h"

using namespace std;

class AlternationTask : public Task {
private:
    int category1;
    int category2;
    bool stop_signal;
public:
    explicit AlternationTask(bool stop_signal);

    AlternationTask(int category1, int category2);

    [[nodiscard]] bool isStopTask() const override;

    const int &getCategory1() const;
    const int &getCategory2() const;
    bool
};


class BiasDetection {
public:
    BiasDetection(const Eigen::MatrixXf &dataset, vector<double> &labels, int &attribute, int &num_categories, const int &num_threads);

    void compute_bias();

    void train_model(AlternationTask &t) const;
private:
    const Eigen::MatrixXf dataset;
    vector<double> labels;
    int attribute;
    int num_categories;
    int num_threads;
};


#endif //SDP_PROJECT_BIASDETECTION_H
