#include "PlotML.h"
#include <matplot/matplot.h>

using namespace std;
using namespace matplot;

// Default constructor
PlotML::PlotML(int fold_number) {
    _fold_number = fold_number;
}

void PlotML::alternation_plot(const string og_attr, const string alternated_attr, const string pred_feat,
                              const Eigen::VectorXf &og_pred, const Eigen::VectorXf &alternated_pred) {
    std::vector<double> x = matplot::iota(0, 1, _fold_number);
    plot(x, og_pred, "--g.");
    hold(on);
    plot(x, alternated_pred, "--b.");
    // grid
    grid(on);
    // text
    title(og_attr+"/"+alternated_attr+" Alternation");
    xlabel(to_string(_fold_number)+"-fold");
    ylabel("Average predicted "+pred_feat);
    vector<string> names {og_attr,og_attr+"=>"+alternated_attr};
    matplot::legend(names);
    show();
    save("./plots/alternation/"+og_attr+"_"+alternated_attr+".png");
}
