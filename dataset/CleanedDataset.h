#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/core"

using namespace std;

class CleanedDataset {
private:
    Eigen::MatrixXf dataset;
    vector<string> classes;
public:
    CleanedDataset();
    const Eigen::MatrixXf &getDataset();
    std::vector<string> getClasses();
    std::vector<double> getLabels();
    int getAttributeIndex(const char *attribute);
};


#endif //SDP_PROJECT_CLEANEDDATASET_H
