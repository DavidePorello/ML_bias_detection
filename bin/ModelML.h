/** An abstract class to be implemented by all other model classes.
 * */
#ifndef SDP_PROJECT_MODELML_H
#define SDP_PROJECT_MODELML_H

#include <Eigen/core>

class ModelML {
    public:
        const Eigen::VectorXf getParams();
        void setParams(const Eigen::VectorXf &params);
        virtual void fit(const Eigen::MatrixXf &train, const Eigen::VectorXf &responses) = 0;
        virtual void predict(const Eigen::MatrixXf &samples, Eigen::VectorXf &predictions) = 0;
    protected:
        Eigen::VectorXf _params;
};


#endif //SDP_PROJECT_MODELML_H
