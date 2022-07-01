#include <iostream>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"
#include "bin/BiasDetection.h"

// CONFIG

#define NUM_THREADS_ALTERNATION 8
char attribute[20] = "gender";
int attribute_num_categories = 2;

//


using namespace std;

int main() {
    CleanedDataset d;
    const Eigen::MatrixXf m = d.getDataset();
    vector<string> classes = d.getClasses();
    vector<double> labels = d.getLabels();
    int attr_index = d.getAttributeIndex(attribute);

    BiasDetection bd(m, labels, attr_index, attribute_num_categories, NUM_THREADS_ALTERNATION);
    bd.compute_bias();

    return 0;
}
