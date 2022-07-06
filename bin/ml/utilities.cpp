// TODO do we keep this class, or remove it in final delivery?

#include <Eigen/Core>
#include <iostream>
#include "ModelML.h"
#include "LinearRegression.h"
#include "PolynomialRegression.h"

using namespace Eigen;

void train_and_predict(const MatrixXf &train, const VectorXf &responses, const MatrixXf &samples,
                       ModelML &model, VectorXf &predictions);
void train_and_predict(MatrixXf &train, VectorXf &responses, const MatrixXf &samples,
                       ModelML &model, VectorXf &predictions);
double kl_divergence(const VectorXf &p, const VectorXf &q, bool unbiased=false);

//Uncomment to try this main
/*int main() {
    // simple test program
    const MatrixXf train {{1,2},{2.3,-3.9},{9,0.8},{8, -0.3}};
    const VectorXf resp {{1,-1,6,5}};
    const MatrixXf sample {{4,9},{0,1},{6,-2}};

    LinearRegression lr;
    PolynomialRegression pr(2);

    VectorXf predictions_lr;
    VectorXf predictions_pr;

    train_and_predict(train, resp, sample, lr, predictions_lr);
    std::cout << "---- LINEAR REGRESSION (expected: 6.5623 -0.21174 2.6021) ----" << std::endl;
    std::cout << predictions_lr << std::endl;

    train_and_predict(train, resp, sample, pr, predictions_pr);
    std::cout << "---- POLYNOMIAL REGRESSION (expected: -10.130 1.3625 2.6243) ----" << std::endl;
    std::cout << predictions_pr << std::endl;

    std::cout << "---- KL DIVERGENCE ----" << std::endl;
    std::cout << "linear -> polynomial: " << kl_divergence(predictions_lr, predictions_pr) << std::endl;
    std::cout << "polynomial -> linear: " << kl_divergence(predictions_pr, predictions_lr) << std::endl;
}*/

/** A wrapper for ModelML train and predict functions.
 *  const version, if we require everything NOT to be changed.
 *  @param train is the training dataset
 *  @param responses is the known value (one for each entry of the dataset)
 *  @param samples is the dataset we want to evaluate
 *  @param model is the type of model we want to use
 *  @param predictions will store the predicted values, one for each sample
*/
void train_and_predict(const MatrixXf &train, const VectorXf &responses, const MatrixXf &samples,
                             ModelML &model, VectorXf &predictions) {
    // train the model
    model.fit(train, responses);
    // predict the values
    model.predict(samples, predictions);
}
/** A wrapper for ModelML train and predict functions.
 *  Here, we avoid copying by modifying the original version of data.
 *  @param train is the training dataset
 *  @param responses is the known value (one for each entry of the dataset)
 *  @param samples is the dataset we want to evaluate
 *  @param model is the type of model we want to use
 *  @param predictions will store the predicted values, one for each sample
*/
void train_and_predict(MatrixXf &train, VectorXf &responses, const MatrixXf &samples,
                       ModelML &model, VectorXf &predictions) {
    // train the model
    model.fit(train, responses);
    // predict the values
    model.predict(samples, predictions);
}
