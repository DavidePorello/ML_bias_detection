#include "CleanedDataset.h"

mat CleanedDataset::getDataset() {
    mat m(10, 40, fill::randu);
    return m;
}

vector<string> CleanedDataset::getClasses() {
    classes.emplace_back("gender");
    classes.emplace_back("race");
    classes.emplace_back("family");
    classes.emplace_back("random");
    classes.emplace_back("blabla");
    classes.emplace_back("agfg");
    classes.emplace_back("asg");
    classes.emplace_back("agafgfhgfg");
    classes.emplace_back("agagsdfg");
    classes.emplace_back("agagsddddddfg");
    return classes;
}

std::vector<double> CleanedDataset::getLabels() {
    vector<double> labels(10, 0);
    for (int i = 0; i < 10; i++)
        labels[i] = 1000 + i;
    return labels;
}

int CleanedDataset::getAttributeIndex(const char *attribute) {
    for (int i = 0; i < classes.size(); i++)
        if (classes[i] == attribute)
            return i;
    return -1;
}
