#include "PolynomialRegression.h"
#include <stdexcept>
using namespace Eigen;

// Default constructor
PolynomialRegression::PolynomialRegression(int degree) {
    if (degree<1 || degree>4)
        throw std::runtime_error("Wrong or not supported degree value.");
    _degree = degree;
    _expanded_cols = 0;
    _lr = LinearRegression();
}

/** This function is used to train a polynomial regression model.
  *  @param train is a matrix having a row for each sample and a column for each feature
  *  @param responses is a column vector storing the expected responses, one for each sample
 * */
void PolynomialRegression::fit(const MatrixXf &train, const VectorXf &responses) {
    // expand features
    if (_degree!=1) {
        MatrixXf exp;
        _poly_features(train, exp);
        _lr.fit(exp, responses);
    } else {
        _lr.fit(train, responses);
    }
}

/** This function is used to predict responses using a polynomial regression model.
  *  @param samples is a matrix having a row for each sample and a column for each feature
  *  @param predictions is a column vector which WILL store the computed responses, one for each sample
 * */
void PolynomialRegression::predict(const MatrixXf &samples, VectorXf &predictions) {
    if (!_lr.is_trained())
        throw std::runtime_error("Model has not been trained yet!");
    if (_degree!=1) {
        MatrixXf expanded;
        _poly_features(samples, expanded);
        _lr.predict(expanded, predictions);
    } else
        _lr.predict(samples, predictions);
}

/** This (private) function computes the feature expansion to support polynomial regression. */
void PolynomialRegression::_poly_features(const MatrixXf &samples, MatrixXf &expanded) {
    // predict the number of features
    if (_expanded_cols == 0) {
        _poly_feat_number(samples.cols());
    }
    expanded.resize(samples.rows(), _expanded_cols);
    // for each sample, generate all combinations
    for (int i=0; i<samples.rows(); i++) {
        // first, we put degree 0
        expanded(i,0) = 1;
        // now, we start generating all combinations
        VectorXi count_deg;
        count_deg = VectorXi::Zero(_degree+1);
        _poly_features_R(samples, expanded, i, count_deg);
    }
}
void PolynomialRegression::_poly_features_R(const MatrixXf &samples, MatrixXf &expanded, int fixed_row, VectorXi &count_deg,
                                            int prev_col, float partial, int curr_degree) {
    float prod;
    for(int j=prev_col; j<samples.cols(); j++) {
        // add partial result to expanded
        prod = partial*samples(fixed_row,j);
        expanded(fixed_row, count_deg[curr_degree] + _expanded_cols_inc[curr_degree]) = prod;
        count_deg[curr_degree]++;
        if(curr_degree<_degree)
            _poly_features_R(samples, expanded, fixed_row, count_deg, j, prod, curr_degree + 1);
    }
}

/** This function computes the number of columns to reshape the expanded features matrix */
void PolynomialRegression::_poly_feat_number(int features) {
    long long res = 1;
    int n = features;
    _expanded_cols_inc.resize(_degree + 1);
    _expanded_cols_inc[0] = 0;
    for (int k=1; k<=_degree; k++) {
        _expanded_cols_inc[k] = static_cast<int>(res);
        res += _fact(n+k-1)/(_fact(k)*_fact(n-1));
    }
    _expanded_cols = res;
}

/** A simple utility to compute a factorial */
long long PolynomialRegression::_fact(int n) {
    long long res = 1;
    for (int i=2; i<=n; i++)
        res *= i;
    return res;
}

