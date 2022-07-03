#ifndef SDP_PROJECT_POLYNOMIALREGRESSION_H
#define SDP_PROJECT_POLYNOMIALREGRESSION_H

#include <Eigen/Core>
#include "ModelML.h"
#include "LinearRegression.h"


class PolynomialRegression: public ModelML {
    public:
        explicit PolynomialRegression(int degree);
        void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses) override;
        void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions) override;
    private:
        int _degree;
        long long _expanded_cols;
        Eigen::VectorXi _expanded_cols_inc;
        LinearRegression _lr;
        void _poly_features(const Eigen::MatrixXf &samples, Eigen::MatrixXf &expanded);
        void _poly_features_R(const Eigen::MatrixXf &samples, Eigen::MatrixXf &expanded, int fixed_row, Eigen::VectorXi &count_deg,
                              int prev_col= 0, float partial= 1, int curr_degree= 1);
        void _poly_feat_number(int features);
        long long _fact(int n);
};


#endif //SDP_PROJECT_POLYNOMIALREGRESSION_H
