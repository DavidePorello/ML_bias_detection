#include "CleanedDataset.h"
#include <fstream>
#include <iostream>

using namespace std;

CleanedDataset::CleanedDataset() {
    vector<Attribute> classes;
    classes.emplace_back("age", 0);
    classes.emplace_back("education", 4).addValues({"High school graduate", "Some college but no degree", "Bachelors degree(BA AB BS)", "Masters degree(MA MS MEng MEd MSW MBA)", "Doctorate degree(PhD EdD)"});
    classes.emplace_back("marital stat", 7).addValues({"Never married", "Married-civilian spouse present", "Married-spouse absent", "Separated", "Divorced", "Widowed"});
    classes.emplace_back("major industry code", 8).addValues({"Entertainment", "Social services", "Agriculture", "Education", "Public administration", "Manufacturing-durable goods", "Manufacturing-nondurable goods", "Wholesale trade", "Retail trade", "Finance insurance and real estate", "Private household services", "Business and repair services", "Personal services except private HH", "Construction", "Medical except hospital", "Transportation", "Utilities and sanitary services", "Mining", "Communications"});
    classes.emplace_back("major occupation code", 9).addValues({"Professional specialty", "Farming forestry and fishing", "Sales", "Adm support including clerical", "Protective services", "Handlers equip cleaners etc ", "Precision production craft & repair", "Technicians and related support", "Machine operators assmblrs & inspctrs", "Transportation and material moving", "Private household services"});
    classes.emplace_back("race", 10).addValues({"White", "Black", "Other", "Indian", "Asian"});
    classes.emplace_back("sex", 12).addValues({"Female", "Male"});
    classes.emplace_back("member of a labor union", 13).addValues({"No", "Yes"});
    classes.emplace_back("full or part time employment stat", 15).addValues({"Children or Armed Forces", "Full-time schedules", "Unemployed part- time", "Not in labor force", "Unemployed full-time"});
    classes.emplace_back("citizenship", 35).addValues({"Native- Born in the United States", "Foreign born- Not a citizen of U S ", "Native- Born in Puerto Rico or U S Outlying", "Native- Born abroad of American Parent(s)", "Foreign born- U S citizen by naturalization"});

    this->attributes = classes;
    this->dataset.resize(14643, classes.size());
    this->labels.resize(14643);
    this->LoadFile("dataset/cleaned-dataset.txt", classes);
}

void CleanedDataset::LoadFile(const char *path, vector<Attribute>& classes) {
    ifstream matrix, label;
    float num;
    int i = 0, j = 0;
    matrix.open(path, ios::in);
    //label.open(pathL, ios::in);
    if(!matrix.is_open()) {
        matrix.close();
        cout << "Creating file " << path << endl;
        Dataset d(classes);
        this->dataset.resize(d.getLength(), classes.size());
        this->labels.resize(d.getLength());
        matrix.open(path, ios::in);
        if(!matrix.is_open()) {
            cout << "Could not open file " << path << endl;
            exit(-1);
        }
    }

    while (matrix >> num) {
        if(j == classes.size()) {
            this->labels(i) = num;
            j = 0;
            i++;
        } else {
            this->dataset(i, j) = num;
            j++;
        }
    }

    matrix.close();
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

int CleanedDataset::getNumberOfValues(int i) const { //continuous attributes don't have values
    return this->attributes[i].getValues().size();
}

const vector<string>& CleanedDataset::getCategories(int i) const {
    return this->attributes[i].getValues();
}