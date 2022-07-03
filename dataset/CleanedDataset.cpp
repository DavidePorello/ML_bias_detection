#include "CleanedDataset.h"

CleanedDataset::CleanedDataset() {
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
}

/*const Eigen::MatrixXf &CleanedDataset::getDataset() {
    return dataset;
}*/

vector<string> CleanedDataset::getClasses() {
    return classes;
}

int CleanedDataset::getAttributeIndex(const char *attribute) {
    for (int i = 0; i < classes.size(); i++)
        if (classes[i] == attribute)
            return i;
    return -1;
}