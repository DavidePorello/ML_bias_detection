#include "Dataset.h"
#include <fstream>
#include <iostream>

using namespace std;

Dataset::Dataset() {
    this->attributes.push_back(Attribute("age", 0));
    this->attributes.push_back(Attribute("education", 4));
    this->attributes.push_back(Attribute("marital stat", 7));
    this->attributes.push_back(Attribute("race", 10));
    this->attributes.push_back(Attribute("hispanic origin", 11));
    this->attributes.push_back(Attribute("sex", 12));
    this->attributes.push_back(Attribute("member of a labor union", 13));
    this->attributes.push_back(Attribute("full or part time employment stat", 15));
    this->attributes.push_back(Attribute("country of birth self", 34));
    this->attributes.push_back(Attribute("citizenship", 35));

    this->attributes[1].addValues({"High school graduate", "Some college but no degree", "Bachelors degree(BA AB BS)", "Masters degree(MA MS MEng MEd MSW MBA)", "Doctorate degree(PhD EdD)"});

    this->raw_dataset = this->LoadFile("dataset/census-income.data");
    vector<RawDataRecord> train = this->LoadFile("dataset/census-income.test");
    this->raw_dataset.insert(this->raw_dataset.end(), train.begin(), train.end());
}

vector<RawDataRecord> Dataset::LoadFile(const char *path) {
    vector<RawDataRecord> dataset;
    ifstream file;
    string line;
    file.open(path, ios::in);
    if(!file.is_open()) {
        cout << "Could not open file " << path << endl;
        exit(-1);
    }
    while(getline(file, line)) {
        RawDataRecord r(line, this->attributes);
        if(r.getRawData().size() == this->attributes.size())
            dataset.push_back(r);
    }
    file.close();
    return dataset;
}

int Dataset::getLength() {
    return this->raw_dataset.size();
}

vector<string> Dataset::getRecord(int i) {
    return this->raw_dataset[i].getRawData();
}

vector<string> Dataset::getAttributes() {
    vector<string> classes;
    for(Attribute& a : this->attributes)
        classes.push_back(a.getName());
    return classes;
}

int Dataset::getAttributeIndex(const string& attribute) {
    int index = 0;
    for(Attribute& a : this->attributes) {
        if(a.getName() == attribute)
            return index;
        index++;
    }
    return -1;
}

string Dataset::getAttribute(int i) {
    return this->attributes[i].getName();
}

