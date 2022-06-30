#include "Dataset.h"
#include <fstream>
#include <iostream>

using namespace std;

Dataset::Dataset() {
    this->raw_dataset = LoadFile("dataset/test.txt"); // TODO remember to handel ebttere the unique pointer
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
    while(getline(file, line)){
        RawDataRecord r(line);
        dataset.emplace_back(r);
        cout << r.raw_data[0] << " " << r.raw_data[1] << " " << r.raw_data[2]<< endl;
    }
    file.close();
    return dataset;
}

