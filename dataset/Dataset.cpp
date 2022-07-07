#include "Dataset.h"
#include <fstream>
#include <iostream>

using namespace std;

Dataset::Dataset(vector<Attribute>& classes) {
    this->attributes = classes;
    this->raw_dataset = this->LoadFile("dataset/census-income.data");
    vector<RawDataRecord> train = this->LoadFile("dataset/census-income.test");
    this->raw_dataset.insert(this->raw_dataset.end(), train.begin(), train.end());
    this->WriteDataset("dataset/cleaned-dataset.txt");
    this->WriteLabels("dataset/labels.txt");
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

void Dataset::WriteDataset(const char *path) {
    int i, j;
    ofstream matrix;
    matrix.open (path, ios::out | ios::trunc);
    if(!matrix.is_open()) {
        cout << "Could not open file " << path << endl;
        exit(-1);
    }

    for(RawDataRecord& r : this->raw_dataset) {
        i = 0;
        for(Attribute& a : this->attributes) {
            if(a.getValues().size() != 0) {
                j = 0;
                for(const string& str : a.getValues()) {
                    if (r.getRawData()[i] == str)
                        matrix << j << " ";
                    j++;
                }
            }
            else
                matrix << r.getRawData()[i] << " ";
            i++;
        }
        matrix << -1 << " ";
    }
    matrix.close();
}

void Dataset::WriteLabels(const char *path) {
    int i, j;
    ofstream label;
    label.open (path, ios::out | ios::trunc);
    if(!label.is_open()) {
        cout << "Could not open file " << path << endl;
        exit(-1);
    }
    for(RawDataRecord& r : this->raw_dataset) {
        label << r.getLabel() << " ";
    }
    label.close();
}

int Dataset::getLength() {
    return this->raw_dataset.size();
}

vector<string> Dataset::getRecord(int i) {
    return this->raw_dataset[i].getRawData();
}