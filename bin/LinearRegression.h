#ifndef SDP_PROJECT_LINEARREGRESSION_H
#define SDP_PROJECT_LINEARREGRESSION_H

#include <Eigen/Core>
#include "ModelML.h"

class LinearRegression: public ModelML{
    public:
        LinearRegression();
        void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses) override;
        void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions) override;
    private:
        float _intercept;
        void _preprocess(Eigen::MatrixXf &train, Eigen::VectorXf &responses,
                         Eigen::RowVectorXf &train_mean, float &resp_mean);
};


#endif //SDP_PROJECT_LINEARREGRESSION_H
