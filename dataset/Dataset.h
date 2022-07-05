#ifndef SDP_PROJECT_DATASET_H
#define SDP_PROJECT_DATASET_H

#include <vector>
#include <string>
#include "RawDataRecord.h"

using namespace std;

class Dataset {
private:
    vector<RawDataRecord> raw_dataset;
    vector<Attribute> attributes;
    vector<RawDataRecord> LoadFile(const char *path);
public:
    Dataset();
    int getLength();
    vector<string> getRecord(int i);
    vector<string> getAttributes();
    int getAttributeIndex(const string& attribute);
    string getAttribute(int i);
};


#endif //SDP_PROJECT_DATASET_H
