#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/core"
#include "DataRecord.h"
#include "Attribute.h"
#include "Dataset.h"

using namespace std;

class CleanedDataset {
private:
    Eigen::MatrixXf dataset;
    vector<Attribute> attributes;
public:
    explicit CleanedDataset(Dataset& d);
    const Eigen::MatrixXf &getDataset();
    vector<string> getAttributes();
    int getAttributeIndex(const string& attribute);
    string getAttribute(int i);
};

#endif //SDP_PROJECT_CLEANEDDATASET_H
