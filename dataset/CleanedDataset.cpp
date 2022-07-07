#include "CleanedDataset.h"
#include <fstream>
#include <iostream>

using namespace std;

CleanedDataset::CleanedDataset() {
    vector<Attribute> classes;
    classes.emplace_back("age", 0);
    classes.emplace_back("education", 4);
    classes.emplace_back("marital stat", 7);
    classes.emplace_back("race", 10);
    classes.emplace_back("hispanic origin", 11);
    classes.emplace_back("sex", 12);
    classes.emplace_back("member of a labor union", 13);
    classes.emplace_back("full or part time employment stat", 15);
    classes.emplace_back("country of birth self", 34);
    classes.emplace_back("citizenship", 35);
    classes[1].addValues({"High school graduate", "Some college but no degree", "Bachelors degree(BA AB BS)", "Masters degree(MA MS MEng MEd MSW MBA)", "Doctorate degree(PhD EdD)"});
    classes[2].addValues({"Never married", "Married-civilian spouse present", "Married-spouse absent", "Separated", "Divorced", "Widowed", "Married-A F spouse present"});
    classes[3].addValues({"White", "Black", "Other", "Amer Indian Aleut or Eskimo", "Asian or Pacific Islander"});
    classes[4].addValues({"Mexican (Mexicano)", "Mexican-American", "Puerto Rican", "Central or South American", "All other", "Other Spanish", "Chicano", "Cuban", "Do not know", "NA"});
    classes[5].addValues({"Female", "Male"});
    classes[6].addValues({"No", "Yes"});
    classes[7].addValues({"Children or Armed Forces", "Full-time schedules", "Unemployed part- time", "Not in labor force", "Unemployed full-time", "PT for non-econ reasons usually FT", "PT for econ reasons usually PT", "PT for econ reasons usually FT"});
    classes[8].addValues({"United-States", "Mexico", "Puerto-Rico", "Peru", "Canada", "South Korea", "India", "Japan", "Haiti", "El-Salvador", "Dominican-Republic", "Portugal", "Columbia", "England", "Thailand", "Cuba", "Laos", "Panama", "China", "Germany", "Vietnam", "Italy", "Honduras", "Outlying-U S (Guam USVI etc)", "Hungary", "Philippines", "Poland", "Ecuador", "Iran", "Guatemala", "Holand-Netherlands", "Taiwan", "Nicaragua", "France", "Jamaica", "Scotland", "Yugoslavia", "Hong Kong", "Trinadad&Tobago", "Greece", "Cambodia", "Ireland"});
    classes[9].addValues({"Native- Born in the United States", "Foreign born- Not a citizen of U S ", "Native- Born in Puerto Rico or U S Outlying", "Native- Born abroad of American Parent(s)", "Foreign born- U S citizen by naturalization"});

    this->attributes = classes;
    this->dataset.resize(22000, classes.size());
    this->labels.resize(22000);
    this->LoadFiles("dataset/cleaned-dataset.txt", "dataset/labels.txt", classes);
}

void CleanedDataset::LoadFiles(const char *pathDB, const char *pathL, vector<Attribute>& classes) {
    ifstream matrix, label;
    float num;
    int i = 0, j = 0;
    matrix.open(pathDB, ios::in);
    label.open(pathL, ios::in);
    if(!matrix.is_open() || !label.is_open()) {
        matrix.close();
        label.close();
        cout << "Creating file " << pathDB << " and " << pathL << endl;
        Dataset d(classes);
        matrix.open(pathDB, ios::in);
        label.open(pathL, ios::in);
        if(!matrix.is_open() || !label.is_open()) {
            cout << "Could not open file " << pathDB << "and/or" << pathL << endl;
            exit(-1);
        }
    }

    while (matrix >> num) {
        if(num != -1) {
            this->dataset(i, j) = num;
            j++;
        }
        else {
            i++;
            j = 0;
        }
    }

    i=0;
    while (label >> num) {
        this->labels(i) = num;
        i++;
    }

    matrix.close();
    label.close();
}

const Eigen::MatrixXf &CleanedDataset::getDataset() {
    return this->dataset;
}

Eigen::VectorXf CleanedDataset::getLabels() {
    return this->labels;
}

vector<string> CleanedDataset::getAttributes() const{
    vector<string> classes;
    for(const Attribute& a : this->attributes)
        classes.push_back(a.getName());
    return classes;
}

int CleanedDataset::getAttributeIndex(const string& attribute) {
    int index = 0;
    for(Attribute& a : this->attributes) {
        if(a.getName() == attribute)
            return index;
        index++;
    }
    return -1;
}

[[maybe_unused]] string CleanedDataset::getAttribute(int i) {
    return this->attributes[i].getName();
}

int CleanedDataset::getNumberOfValues(int i) const {
    return this->attributes[i].getValues().size();
}