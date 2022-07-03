#include <iostream>
#include <memory>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"

using namespace std;

int main() {

    Dataset d;
    cout << d.getLength() << endl;
    for(string& str : d.getRecord(0))
        cout << str << endl;

    return 0;
}
