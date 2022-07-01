#ifndef SDP_PROJECT_CLEANEDDATASET_H
#define SDP_PROJECT_CLEANEDDATASET_H

#include "armadillo"

using namespace std;
using namespace arma;

class CleanedDataset {
private:
    vector<string> classes;
public:
    mat getDataset();
    std::vector<string> getClasses();
    std::vector<double> getLabels();
    int getAttributeIndex(const char *attribute);
};


#endif //SDP_PROJECT_CLEANEDDATASET_H
