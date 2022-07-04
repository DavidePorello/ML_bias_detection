#include "CleanedDataset.h"

CleanedDataset::CleanedDataset() {
    classes.emplace("age", initVector<string>);
    classes.emplace_back("education");
    classes.emplace_back("marital stat");
    classes.emplace_back("race");
    classes.emplace_back("hispanic origin");
    classes.emplace_back("sex");
    classes.emplace_back("reason for unemployment");
    classes.emplace_back("full or part time employment stat");
    classes.emplace_back("country of birth self");
    classes.emplace_back("citizenship");
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