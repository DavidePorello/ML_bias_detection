#ifndef SDP_PROJECT_POLYNOMIALREGRESSION_H
#define SDP_PROJECT_POLYNOMIALREGRESSION_H

#include <Eigen/Core>
#include "ModelML.h"

class PolynomialRegression: public ModelML {
    public:
        PolynomialRegression(int degree);
        void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses);
        void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions);
    private:
        int _degree;
        long long _expandedCols;
        Eigen::VectorXi _expandedColsInc;
        void polyFeatures(const Eigen::MatrixXf &samples, Eigen::MatrixXf &expanded);
        void polyFeatures_R(const Eigen::MatrixXf &samples, Eigen::MatrixXf &expanded, int fixed_row, Eigen::VectorXi &count_deg,
                            int prev_col=0, float partial=1, int curr_degree=1);
        void polyFeatNumber(int features);
        long long fact(int n);
};


#endif //SDP_PROJECT_POLYNOMIALREGRESSION_H
