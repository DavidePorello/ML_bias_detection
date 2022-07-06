#ifndef SDP_PROJECT_DATASET_H
#define SDP_PROJECT_DATASET_H

#include <vector>
#include <string>
#include "RawDataRecord.h"

using namespace std;

class Dataset {
private:
    vector<RawDataRecord> raw_dataset;
    vector<float> labels;
    vector<Attribute> attributes;
    vector<RawDataRecord> LoadFile(const char *path);
    void WriteDataset(const char *path);
    void WriteLabels(const char *path);
public:
    explicit Dataset(vector<Attribute>& classes);
    int getLength();
    vector<string> getRecord(int i);
};


#endif //SDP_PROJECT_DATASET_H
