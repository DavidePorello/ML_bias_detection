#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/core"
#include "DataRecord.h"
#include "unordered_map"


using namespace std;

class CleanedDataset {
private:
    //Eigen::MatrixXf dataset;
    vector<DataRecord> dataset;
    unordered_map<string, vector<string>> classes;
public:
    CleanedDataset();
    const Eigen::MatrixXf &getDataset();
    std::vector<string> getClasses();
    int getAttributeIndex(const char *attribute);
};



#endif //SDP_PROJECT_CLEANEDDATASET_H