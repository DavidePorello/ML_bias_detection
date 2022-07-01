#ifndef SDP_PROJECT_LINEARREGRESSION_H
#define SDP_PROJECT_LINEARREGRESSION_H

#include <Eigen/core>
#include "ModelML.h"

class LinearRegression: public ModelML{
    public:
        //const arma::vec getParams();
        //void setParams(arma::vec params);
        void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses);
        void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions);
};


#endif //SDP_PROJECT_LINEARREGRESSION_H
