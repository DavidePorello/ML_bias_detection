#include "Attribute.h"

Attribute::Attribute(string name, int index) {
    this->name = name;
    this->index = index;
}

void Attribute::addValues(vector<string> values) {
    this->values.assign(values.begin(), values.end());
}

string Attribute::getName() {
    return this->name;
}

int Attribute::getIndex() {
    return this->index;
}

vector<string> Attribute::getValues() {
    return this->values;
}