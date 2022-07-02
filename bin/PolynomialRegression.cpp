#include "PolynomialRegression.h"
#include <stdexcept>
#include "Eigen/LU"
using namespace Eigen;

// Default constructor
PolynomialRegression::PolynomialRegression(int degree) {
    if (degree<1 || degree>4)
        throw std::runtime_error("Wrong or not supported degree value.");
    _degree = degree;
    _expandedCols = 0;
}

/** This function is used to train a polynomial regression model.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void PolynomialRegression::fit(const MatrixXf &train, const VectorXf &responses) {
    // expand features
    if (_degree!=1) {
        MatrixXf exp;
        polyFeatures(train, exp);
        auto exp_T = exp.transpose();
        _params = (exp_T*exp).inverse()*exp_T*responses;
    } else {
        auto train_T = train.transpose();
        _params = (train_T*train).inverse()*train_T*responses;
    }
}

/** This function is used to predict responses using a polynomial regression model.
  *  @param samples is a matrix having a row for each sample and a column for each feature
  *  @param predictions is a column vector which WILL store the computed responses, one for each sample
 * */
void PolynomialRegression::predict(const MatrixXf &samples, VectorXf &predictions) {
    if (_params.size()==0)
        throw std::runtime_error("Model has not been trained yet!");
    if (_degree!=1) {
        MatrixXf expanded;
        polyFeatures(samples, expanded);
        predictions = expanded*_params;
    } else
        predictions = samples*_params;
}

void PolynomialRegression::polyFeatures(const MatrixXf &samples, MatrixXf &expanded) {
    // predict the number of features
    if (_expandedCols==0) {
        polyFeatNumber(samples.cols());
    }
    expanded.resize(samples.rows(), _expandedCols);
    // for each sample, generate all combinations
    for (int i=0; i<samples.rows(); i++) {
        // first, we put degree 0
        expanded(i,0) = 1;
        // now, we start generating all combinations
        VectorXi count_deg;
        count_deg = VectorXi::Zero(_degree+1);
        polyFeatures_R(samples, expanded, i, count_deg);
    }
}

void PolynomialRegression::polyFeatures_R(const MatrixXf &samples, MatrixXf &expanded, int fixed_row, VectorXi &count_deg,
                                          int prev_col, float partial, int curr_degree) {
    float prod;
    for(int j=prev_col; j<samples.cols(); j++) {
        // add partial result to expanded
        prod = partial*samples(fixed_row,j);
        expanded(fixed_row, count_deg[curr_degree]+_expandedColsInc[curr_degree]) = prod;
        count_deg[curr_degree]++;
        if(curr_degree<_degree)
            polyFeatures_R(samples, expanded, fixed_row, count_deg, j, prod, curr_degree+1);
    }
}

/** This function computes the number of columns to reshape the expanded features matrix */
void PolynomialRegression::polyFeatNumber(int features) {
    long long res = 1;
    int n = features;
    _expandedColsInc.resize(_degree+1);
    _expandedColsInc[0] = 0;
    for (int k=1; k<=_degree; k++) {
        _expandedColsInc[k] = static_cast<int>(res);
        res += fact(n+k-1)/(fact(k)*fact(n-1));
    }
    _expandedCols = res;
}

/** A simple utility to compute a factorial */
long long PolynomialRegression::fact(int n) {
    long long res = 1;
    for (int i=2; i<=n; i++)
        res *= i;
    return res;
}

