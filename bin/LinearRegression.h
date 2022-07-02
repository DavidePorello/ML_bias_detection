#ifndef SDP_PROJECT_LINEARREGRESSION_H
#define SDP_PROJECT_LINEARREGRESSION_H

#include <Eigen/Core>
#include "ModelML.h"

class LinearRegression: public ModelML{
    public:
        void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses) override;
        void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions) override;
    private:
        void _preprocess(Eigen::MatrixXf &samplesCopy, Eigen::VectorXf &labelsCopy);
};


#endif //SDP_PROJECT_LINEARREGRESSION_H
