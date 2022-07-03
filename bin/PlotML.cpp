#include "PlotML.h"
#include <sciplot/sciplot.hpp>
#include <numeric>

using namespace std;
using namespace sciplot;

/** The constructor for the PlotML object.
 * @param fold_number the number of folds we used (eg, 10 if we did 10-folds)
 */
PlotML::PlotML(int fold_number) {
    _fold_number = fold_number;
}
/** Generates the alternation figure and saves it as a pdf file.
 * @param og_attr the value of the original attribute (eg, female)
 * @param alternated_attr the value of the alternated value (eg, male if we did female=>male)
 * @param pred_feat the meaning of the predicted feature (eg, wage)
 * @param og_pred the predicted values WITHOUT alternation
 * @param alternated_pred the predicted values WITH alternation
 */
void PlotML::alternation_plot(const string &og_attr, const string &alternated_attr, const string &pred_feat,
                              const Eigen::VectorXf &og_pred, const Eigen::VectorXf &alternated_pred) const {
    std::vector<int> x(_fold_number);
    iota(x.begin(), x.end(), 1);

    // create a Plot object
    Plot plot;
    // set labels
    plot.xlabel(to_string(_fold_number)+"-fold");
    plot.ylabel("Average predicted "+pred_feat);
    // Set the legend to be on the bottom along the horizontal
    plot.legend()
        .atOutsideBottom()
        .displayHorizontal();

    // plot curves
    plot.drawBrokenCurveWithPoints(x, og_pred).label(og_attr);
    plot.drawBrokenCurveWithPoints(x, alternated_pred).label(og_attr+"=>"+alternated_attr);

    // set grid
    plot.grid().show();

    // set figure
    Figure fig = {{plot}};
    // set palette
    fig.palette("paired");
    // set title
    fig.title(og_attr+"/"+alternated_attr+" Alternation");
    // show and save figure
    fig.save("./plots/alternation_"+og_attr+"_"+alternated_attr+".pdf");
}
/** Generates the bias evaluation plot and saves it as a pdf file.
 * @param p1 a value of the protected attribute (eg, female)
 * @param p2 another value of the protected attribute (eg, male)
 * @param kl_div1 KL divergence results for p1 (eg, KL(female, female->male) if p1 = female)
 * @param kl_div2 KL divergence results for p2 (eg, KL(male, male->female) if p2 = male)
 */
void PlotML::bias_evaluation_plot(const string &p1, const string &p2, const Eigen::VectorXf &kl_div1,
                                  const Eigen::VectorXf &kl_div2) const {
    std::vector<int> x(_fold_number);
    iota(x.begin(), x.end(), 1);

    // create a Plot object
    Plot plot;
    // set labels
    plot.xlabel(to_string(_fold_number)+"-fold");
    plot.ylabel("KL Divergence");
    // Set the legend to be on the bottom along the horizontal
    plot.legend()
            .atOutsideBottom()
            .displayHorizontal();

    // plot curves
    plot.drawBrokenCurveWithPoints(x, kl_div1).label("Bias against " + p1);
    plot.drawBrokenCurveWithPoints(x, kl_div2).label("Bias against " + p2);

    // set grid
    plot.grid().show();
    // set figure
    Figure fig = {{plot}};
    // set palette
    fig.palette("set1");
    // set title
    fig.title("Bias Evaluation Using KL Divergence "+p1+"-"+p2);
    // show and save figure
    fig.save("./plots/bias_evaluation_"+p1+"_"+p2+".pdf");
}

