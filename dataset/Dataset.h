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
};


#endif //SDP_PROJECT_DATASET_H
