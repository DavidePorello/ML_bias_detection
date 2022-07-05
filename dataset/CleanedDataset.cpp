#include "CleanedDataset.h"

CleanedDataset::CleanedDataset() {
    // Eigen::MatrixXf is automatically saved in heap, since the dataset can be quite large
    this->dataset = Eigen::MatrixXf{{0, 2, 3, 4, 5, 6, 7, 8, 9, 0},
                                    {1, 3, 3, 4, 5, 6, 7, 8, 9, 0},
                                    {0, 4, 6, 4, 5, 6, 7, 8, 9, 9},
                                    {0, 5, 3, 2, 5, 6, 7, 8, 9, 0},
                                    {1, 6, 2, 4, 3, 2, 7, 2, 3, 0},
                                    {1, 7, 4, 4, 5, 6, 7, 8, 9, 0},
                                    {0, 8, 3, 4, 5, 6, 1, 8, 9, 0},
                                    {1, 9, 3, 4, 5, 6, 7, 8, 9, 5},
                                    {0, 0, 3, 4, 6, 6, 7, 8, 9, 0},
                                    {1, 2, 5, 3, 5, 7, 0, 1, 8, 0},
                                    {1, 3, 3, 4, 5, 6, 7, 8, 9, 0},
                                    {1, 5, 3, 4, 5, 9, 7, 8, 9, 0}};
}

/**
 * return a preprocessed dataset
 * @return dataset where each column is an attribute
 */
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

Eigen::VectorXf CleanedDataset::getLabels() {
    Eigen::VectorXf labels = Eigen::VectorXf::LinSpaced(12, 1000, 1011);
    return labels;
}

int CleanedDataset::getAttributeIndex(const string attribute) {
    for (int i = 0; i < classes.size(); i++)
        if (classes[i] == attribute)
            return i;
    return -1;
}
