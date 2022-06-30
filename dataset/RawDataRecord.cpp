#include "RawDataRecord.h"
#include <sstream>

RawDataRecord::RawDataRecord(const string &line) {
    stringstream s(line);
    string word;

    while(getline(s,word, ',')) {
        this->raw_data.emplace_back(word);
    }
}