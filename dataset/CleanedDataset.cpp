#include "CleanedDataset.h"

using namespace std;

CleanedDataset::CleanedDataset(Dataset& d) {

}

const Eigen::MatrixXf &CleanedDataset::getDataset() {
    return this->dataset;
}

vector<string> CleanedDataset::getAttributes() {
    vector<string> classes;
    for(Attribute& a : this->attributes)
        classes.push_back(a.getName());
    return classes;
}

int CleanedDataset::getAttributeIndex(const string& attribute) {
    int index = 0;
    for(Attribute& a : this->attributes) {
        if(a.getName() == attribute)
            return index;
        index++;
    }
    return -1;
}

string CleanedDataset::getAttribute(int i) {
    return this->attributes[i].getName();
}