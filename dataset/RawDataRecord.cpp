#include "RawDataRecord.h"
#include <sstream>
#include <iostream>

using namespace std;

RawDataRecord::RawDataRecord(const string &line) {
    stringstream s(line);
    string word;
    int pos;
    int index = 0;

    indexes.emplace_back(0);
    indexes.emplace_back(4);
    indexes.emplace_back(7);
    indexes.emplace_back(10);
    indexes.emplace_back(11);
    indexes.emplace_back(12);
    indexes.emplace_back(14);
    indexes.emplace_back(15);
    indexes.emplace_back(34);
    indexes.emplace_back(35);

    while(getline(s,word, ',')) {
        // exit before last fake parameter
        if(word ==" - 50000.") {
            break;
        }
        // remove leading spaces
        pos = word.find_first_not_of(' ');
        word = word.substr(pos != string::npos ? pos : 0);
        // add to string vector
        for(int i = 0; i < indexes.size(); i++)
            if(indexes[i] == index)
                this->raw_data.emplace_back(word);

        index++;
    }
}

bool RawDataRecord::isPrunable() {
    for(string& str : this->raw_data)
        if(str == "Not in universe" || str == "?")
            return true;
    return false;
}

vector<string> RawDataRecord::getRawData() {
    return raw_data;
}
