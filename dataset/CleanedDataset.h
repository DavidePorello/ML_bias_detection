#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "Eigen/core"
#include "Attribute.h"
#include "Dataset.h"

using namespace std;

class CleanedDataset {
private:
    Eigen::MatrixXf dataset;
    Eigen::VectorXf labels;
    vector<Attribute> attributes;
    void LoadFiles(const char *pathDB, const char *pathL, vector<Attribute>& classes);
public:
    explicit CleanedDataset();
    const Eigen::MatrixXf &getDataset();
    Eigen::VectorXf getLabels();
    vector<string> getAttributes();
    int getAttributeIndex(const string& attribute);
    string getAttribute(int i);
    int getNumberOfValues(int i);
};

#endif //SDP_PROJECT_CLEANEDDATASET_H
