#include "Dataset.h"
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

Dataset::Dataset() {
    this->raw_dataset = LoadFile("dataset/census-income.test"); // TODO also add records from train file
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
        if (!r.is_prunable())
            dataset.emplace_back(r);
    }
    file.close();
    return dataset;
}

int Dataset::getLength() {
    return raw_dataset.size();
}


