#ifndef SDP_PROJECT_DATA_TYPES_H
#define SDP_PROJECT_DATA_TYPES_H

#include "Eigen/Core"
#include <future>

using namespace std;

// TODO remove and replace with a simple Eigen::MatrixXf?
/**
 * @var dataset: the future of an alternated dataset
 * @var a1: numerical value of first alternated category (e.g. male->0)
 * @var a2: numerical value of second alternated category (e.g. female->1)
 */
class AlternatedDataset{
public:
    Eigen::MatrixXf dataset;
    int a1, a2;
    AlternatedDataset() : dataset(), a1(-1), a2(-1) {};
    AlternatedDataset(Eigen::MatrixXf dataset, int a1, int a2) : dataset(move(dataset)), a1(a1), a2(a2) {};
};


#endif //SDP_PROJECT_DATA_TYPES_H
