#include <memory>
#include "CleanedDataset.h"

CleanedDataset::CleanedDataset() {
    this->dataset = Eigen::MatrixXf{{0, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1},
                                    {1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1},
                                    {2, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1},
                                    {3, 2, 3, 4, 5, 6, 7, 8, 9, 0, 2},
                                    {4, 2, 3, 4, 5, 6, 7, 8, 9, 0, 4},
                                    {5, 2, 3, 4, 5, 6, 7, 8, 9, 0, 7},
                                    {6, 2, 3, 4, 5, 6, 7, 8, 9, 0, 9},
                                    {7, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1},
                                    {8, 2, 3, 4, 5, 6, 7, 8, 9, 0, 76},
                                    {9, 2, 3, 4, 5, 6, 7, 8, 9, 0, 2405}};
}

const Eigen::MatrixXf &CleanedDataset::getDataset() {
    return dataset;
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
