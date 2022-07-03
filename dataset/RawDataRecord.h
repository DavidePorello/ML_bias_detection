#ifndef SDP_PROJECT_RAWDATARECORD_H
#define SDP_PROJECT_RAWDATARECORD_H

#include <vector>
#include <string>

using namespace std;

class RawDataRecord {
private:
    vector<string> raw_data;
public:
    explicit RawDataRecord(const string& line);
    bool isPrunable();
};


#endif //SDP_PROJECT_RAWDATARECORD_H
