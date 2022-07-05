#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/Core"

using namespace std;

class CleanedDataset {
private:
    Eigen::MatrixXf dataset;
    vector<string> classes;
public:
    CleanedDataset();
    const Eigen::MatrixXf &getDataset();
    std::vector<string> getClasses();
    Eigen::VectorXf getLabels();
    int getAttributeIndex(const char *attribute);
};


#endif //SDP_PROJECT_CLEANEDDATASET_H
