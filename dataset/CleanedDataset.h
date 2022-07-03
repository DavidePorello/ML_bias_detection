#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/core"
#include "DataRecord.h"

using namespace std;

class CleanedDataset {
private:
    //Eigen::MatrixXf dataset;
    vector<DataRecord> dataset;
    vector<string> classes;
public:
    CleanedDataset();
    const Eigen::MatrixXf &getDataset();
    std::vector<string> getClasses();
    int getAttributeIndex(const char *attribute);
};



#endif //SDP_PROJECT_CLEANEDDATASET_H
