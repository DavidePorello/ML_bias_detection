#include <Eigen/Core>
#include "PlotML.h"

int main() {
    PlotML plot(10);
    Eigen::VectorXf og {{900,901,999,850,950, 930,903,888,900,1000}};
    Eigen::VectorXf alt {{1200,1209,1100,1350,1290,1250,1190,1200,1290,1300}};
    plot.alternation_plot("Female", "Male", "wage", og, alt);
}