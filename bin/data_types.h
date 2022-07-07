#ifndef SDP_PROJECT_DATA_TYPES_H
#define SDP_PROJECT_DATA_TYPES_H

#include "Eigen/Core"

class AlternatedMatrix {
public:
    Eigen::MatrixXf dataset;
    int a1, a2;

    AlternatedMatrix(Eigen::MatrixXf dataset, int a1, int a2) : dataset(dataset), a1(a1), a2(a2) {};
    AlternatedMatrix() : a1(-1), a2(-1) {};
};

#endif //SDP_PROJECT_DATA_TYPES_H
