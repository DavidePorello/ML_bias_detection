#include <iostream>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"

using namespace std;

int main() {

    Dataset d;
    cout << "Raw dataset: " << d.getLength() << endl << d.getRecord(0).size() << endl;
    int i = 0;
    for(string& str : d.getRecord(0)) {
        cout << d.getAttribute(i) << ": " << str << endl;
        i++;
    }

    return 0;
}
