#include "RawDataRecord.h"
#include <sstream>
#include <iostream>

using namespace std;

RawDataRecord::RawDataRecord(const string &line, vector<Attribute> attributes) {
    stringstream s(line);
    string word;
    int pos;
    int index = 0;

    while(getline(s,word, ',')) {
        // exit before last fake parameter
        if(word == " - 50000.")
            break;

        // remove leading spaces
        pos = word.find_first_not_of(' ');
        word = word.substr(pos != string::npos ? pos : 0);

        if(word == "Not in universe" || word == "?" || word == "NA") {
            index++;
            continue;
        }

        // add to string vector
        for(Attribute& a: attributes) {
            if(a.getIndex() == index) {
                if(a.getValues().size() != 0) {
                    for(const string& str : a.getValues())
                        if(word == str)
                            this->raw_data.push_back(word);
                } else
                    this->raw_data.push_back(word);
            }
            if(index == 24)
                this->label = word;
        }
        index++;
    }
}

vector<string> RawDataRecord::getRawData() {
    return raw_data;
}

string RawDataRecord::getLabel() {
    return label;
}