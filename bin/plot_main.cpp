#include <Eigen/Core>
#include "PlotML.h"

int main() {
    PlotML plot(10);
    Eigen::VectorXf og {{900,901,999,850,950, 930,903,888,900,1000}};
    Eigen::VectorXf alt {{1200,1209,1100,1350,1290,1250,1190,1200,1290,1300}};
    plot.alternation_plot("Female", "Male", "wage", og, alt);

    Eigen::VectorXf kl1 {{1,0.95,0.8,0.83,0.89,1.05,0.97,0.96,0.97,1}};
    Eigen::VectorXf kl2 {{0.4,0.45,0.39,0.33,0.39,0.42,0.47,0.36,0.37,0.45}};
    plot.bias_evaluation_plot("Female", "Male", kl1, kl2);
    return 0;
}