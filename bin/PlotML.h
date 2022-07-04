#ifndef SDP_PROJECT_PLOTML_H
#define SDP_PROJECT_PLOTML_H

#include <Eigen/Core>

class PlotML {
    public:
        explicit PlotML(int fold_number);
        void alternation_plot(const std::string &og_attr, const std::string &alternated_attr, const std::string &pred_feat,
                              const Eigen::VectorXf &og_pred, const Eigen::VectorXf &alternated_pred) const;
        void bias_evaluation_plot(const std::string &p1, const std::string &p2, const Eigen::VectorXf &kl_div1,
                                  const Eigen::VectorXf &kl_div2) const;
    private:
        int _fold_number;
};


#endif //SDP_PROJECT_PLOTML_H
