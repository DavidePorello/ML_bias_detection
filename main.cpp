#include <iostream>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"
#include "dataset/Attribute.h"

using namespace std;

int main() {

    CleanedDataset c;
    for(int i = 0 ; i < 10; i++) {
        for (int j = 0; j < c.getDataset().cols(); j++)
            cout << c.getDataset()(i, j) << " ";
        cout << c.getLabels()(i);
        cout << endl;
    }

    return 0;
}
