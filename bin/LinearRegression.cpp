#include "LinearRegression.h"
#include <stdexcept>
#include <Eigen/Dense>
using namespace Eigen;

/** This function is used to train a multiple linear regression model.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void LinearRegression::fit(const MatrixXf &train, const VectorXf &responses) {
    // preprocess data
    MatrixXf trainCopy(train);
    VectorXf responsesCopy(responses);
    _preprocess(trainCopy, responsesCopy);
    // solve using the least square method
    _params = trainCopy.colPivHouseholderQr().solve(responsesCopy);
}

/** This function is used to predict responses using a multiple linear regression model.
  *  @param samples is a matrix having a row for each sample and a column for each feature
  *  @param predictions is a column vector which WILL store the computed responses, one for each sample
 * */
void LinearRegression::predict(const MatrixXf &samples, VectorXf &predictions) {
    if (_params.size()==0)
        throw std::runtime_error("Model has not been trained yet!");
    predictions = samples*_params;
}

/** This function is used to preprocess data. Since we are working with a liear algorithm, it will surely require
 *  having centered data (ie, we remove the mean from each feature).
 */
void LinearRegression::_preprocess(MatrixXf &samplesCopy, VectorXf &labelsCopy) {
    int r, c;
    double mu;
    // first, we focus on samples
    for (c=0; c<samplesCopy.cols(); c++) {
        // extract the column
        auto col = samplesCopy.col(c);
        // compute its mean
        mu = col.mean();
        // remove the mean from the column
        for(r=0; r<samplesCopy.rows(); r++)
            col[r] -= mu;
    }
    // then, we focus on labels
    mu = labelsCopy.mean();
    for(r=0; r<labelsCopy.rows(); r++)
        labelsCopy[r] -= mu;
}

