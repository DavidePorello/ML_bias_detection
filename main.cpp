#include <iostream>
#include <memory>
#include "dataset/Dataset.h"
#include "dataset/CleanedDataset.h"

using namespace std;

int main() {

    Dataset d;
    CleanedDataset c;
    cout << d.getLength() << endl;
    cout << d.getRecord(0).size() << endl;
    int i = 0;
    for(string& str : d.getRecord(0)){
        cout << c.getClasses()[i] << ": " << str << endl;
        i++;
    }



    return 0;
}
