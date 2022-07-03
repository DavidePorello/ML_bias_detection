#include "Dataset.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Dataset::Dataset() {
    this->raw_dataset = this->LoadFile("dataset/census-income.data");
    vector<RawDataRecord> train = this->LoadFile("dataset/census-income.test");
    this->raw_dataset.insert(this->raw_dataset.end(), train.begin(), train.end());
}

vector<RawDataRecord> Dataset::LoadFile(const char *path) {
    vector<RawDataRecord> dataset;
    ifstream file;
    string line;
    file.open(path, ios::in);
    if (!file.is_open()) {
        cout << "Could not open file " << path << endl;
        exit(-1);
    }
    while (getline(file, line)) {
        RawDataRecord r(line);
        //if (!r.isPrunable())
            dataset.emplace_back(r);
    }
    file.close();
    return dataset;
}

int Dataset::getLength() {
    return raw_dataset.size();
}

vector<string> Dataset::getRecord(int i) {
    return raw_dataset[i].getRawData();
}

