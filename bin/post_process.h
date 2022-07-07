#ifndef SDP_PROJECT_POST_PROCESS_H
#define SDP_PROJECT_POST_PROCESS_H

#include "Eigen/Core"
#include <future>
#include <vector>
#include "plot/PlotML.h"
#include "../dataset/CleanedDataset.h"

using namespace std;

void process_results(const CleanedDataset& d, const PlotML& plotter, Eigen::MatrixXf& true_means, Eigen::MatrixXf& true_stddevs, vector<Eigen::MatrixXf>& alt_means, vector<Eigen::MatrixXf>& alt_stddevs, const string& label_name, int attr_index);

#endif //SDP_PROJECT_POST_PROCESS_H
