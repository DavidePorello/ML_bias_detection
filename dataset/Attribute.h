#ifndef SDP_PROJECT_ATTRIBUTE_H
#define SDP_PROJECT_ATTRIBUTE_H

#include <vector>
#include <string>

using namespace std;

class Attribute {
private:
    vector<string> values;
    string name;
    int index;
public:
    Attribute(string name, int index);
    void addValues(vector<string> values);
    string getName() const;
    int getIndex();
    const vector<string>& getValues() const;
};

#endif //SDP_PROJECT_ATTRIBUTE_H
