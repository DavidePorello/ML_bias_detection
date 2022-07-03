#ifndef SDP_PROJECT_DATARECORD_H
#define SDP_PROJECT_DATARECORD_H

#include "Eigen/core"

using namespace std;

class DataRecord {
private:
    //Eigen::VectorXf data;
    vector<string> raw_data;
public:
    bool isPrunable();
};


#endif //SDP_PROJECT_DATARECORD_H
