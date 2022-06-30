#ifndef SDP_PROJECT_RAWDATARECORD_H
#define SDP_PROJECT_RAWDATARECORD_H

#include <vector>
#include <string>

using namespace std;

class RawDataRecord {
public:
    vector<string> raw_data; // TODO make private
    explicit RawDataRecord(const string& line);
    bool is_prunable();
};


#endif //SDP_PROJECT_RAWDATARECORD_H
