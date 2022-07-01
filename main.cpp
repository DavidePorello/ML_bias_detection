#include <iostream>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"

using namespace std;

int main() {
    CleanedDataset d;
    mat m = d.loadDataset();



    return 0;
}
