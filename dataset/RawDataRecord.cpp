#include "RawDataRecord.h"
#include <sstream>

RawDataRecord::RawDataRecord(const string &line) {
    stringstream s(line);
    string word;
    int pos;

    while(getline(s,word, ',')) {
        // exit before last fake parameter
        if(word == "- 50000.")
            break;
        // remove leading spaces
        pos = word.find_first_not_of(' ');
        word = word.substr(pos != string::npos ? pos : 0);
        // add to string vector
        this->raw_data.emplace_back(word);
    }
}

bool RawDataRecord::is_prunable() {
    for(string& str : this->raw_data)
        if(str == "Not in universe")
            return true;
    return false;
}
