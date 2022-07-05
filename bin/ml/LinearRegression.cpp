#include "LinearRegression.h"
#include <stdexcept>
#include <Eigen/Dense>
using namespace Eigen;

// Default constructor
LinearRegression::LinearRegression() {
    _intercept = 0.0;
}

/** This function is used to train a multiple linear regression model.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void LinearRegression::fit(MatrixXf &train, VectorXf &responses) {
    // preprocess data
    RowVectorXf train_mean;
    train_mean.resize(train.cols());
    float resp_mean;
    _preprocess(train, responses, train_mean, resp_mean);
    // solve using the least square method
    _params = train.completeOrthogonalDecomposition().solve(responses);
    // set intercept
    _intercept = resp_mean - train_mean*_params;
}


/** This function is used to train a multiple linear regression model. const version.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void LinearRegression::fit(const MatrixXf &train, const VectorXf &responses) {
    //copy data
    MatrixXf train_copy(train);
    VectorXf responses_copy(responses);
    // preprocess data
    RowVectorXf train_mean;
    train_mean.resize(train.cols());
    float resp_mean;
    _preprocess(train_copy, responses_copy, train_mean, resp_mean);
    // solve using the least square method
    _params = train_copy.completeOrthogonalDecomposition().solve(responses_copy);
    // set intercept
    _intercept = resp_mean - train_mean*_params;
}

/** This function is used to predict responses using a multiple linear regression model.
  *  @param samples is a matrix having a row for each sample and a column for each feature
  *  @param predictions is a column vector which WILL store the computed responses, one for each sample
 * */
void LinearRegression::predict(const MatrixXf &samples, VectorXf &predictions) {
    if (!is_trained())
        throw std::runtime_error("Model has not been trained yet!");
    predictions = samples*_params;
    for (float &value : predictions)
        value += _intercept;
}

/** This function returns true if the model has been trained, false otherwise. */
bool LinearRegression::is_trained() {
    return _params.size()!=0;
}

/** This function is used to preprocess data. Since we are working with a linear algorithm, it will surely require
 *  having centered data (ie, we remove the mean from each feature).
 */
void LinearRegression::_preprocess(MatrixXf &train, VectorXf &responses, RowVectorXf &train_mean, float &resp_mean) {
    int r, c;
    // first, we focus on samples
    for (c=0; c < train.cols(); c++) {
        // extract the column
        auto col = train.col(c);
        // compute its mean
        train_mean[c] = col.mean();
        // remove the mean from the column
        for(r=0; r < train.rows(); r++)
            col[r] -= train_mean[c];
    }
    // then, we focus on labels
    resp_mean = responses.mean();
    for(r=0; r < responses.rows(); r++)
        responses[r] -= resp_mean;
}

