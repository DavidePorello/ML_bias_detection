#ifndef SDP_PROJECT_POST_PROCESS_H
#define SDP_PROJECT_POST_PROCESS_H

#include "Eigen/Core"
#include <future>
#include <vector>
#include "plot/PlotML.h"
#include "../dataset/CleanedDataset.h"

using namespace std;

void process_results(const CleanedDataset& d, const PlotML& plotter, vector<vector<float>>& true_preds, vector<vector<float>>& alt_preds, const string& label_name);

#endif //SDP_PROJECT_POST_PROCESS_H
