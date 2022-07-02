#include "LinearRegression.h"
#include <stdexcept>
#include "Eigen/LU"
using namespace Eigen;

/** This function is used to train a multiple linear regression model.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void LinearRegression::fit(const MatrixXf &train, const VectorXf &responses) {
    auto train_T = train.transpose();
    _params = (train_T*train).inverse()*train_T*responses;
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

