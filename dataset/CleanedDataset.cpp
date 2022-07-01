#include "CleanedDataset.h"

mat CleanedDataset::loadDataset() {
    mat dataset;
    bool loaded = mlpack::data::Load("dataset/test.csv", dataset);
    if (!loaded)
        return nullptr;
    mat m(10,40, fill::randu);
    return m;
}
