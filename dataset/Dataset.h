#ifndef SDP_PROJECT_DATASET_H
#define SDP_PROJECT_DATASET_H

#include <vector>
#include <memory>
#include "RawDataRecord.h"

using namespace std;

class Dataset {
private:
    vector<RawDataRecord> raw_dataset;
    vector<RawDataRecord> LoadFile(const char *path);
public:
    Dataset();
    int getLength();
};


#endif //SDP_PROJECT_DATASET_H
