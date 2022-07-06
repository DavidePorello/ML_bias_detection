#ifndef SDP_PROJECT_RAWDATARECORD_H
#define SDP_PROJECT_RAWDATARECORD_H

#include <vector>
#include <string>
#include "Attribute.h"

using namespace std;

class RawDataRecord {
private:
    vector<string> raw_data;
    string label;
public:
    RawDataRecord(const string& line, vector<Attribute> attributes);
    vector<string> getRawData();
    string getLabel();
};


#endif //SDP_PROJECT_RAWDATARECORD_H
