#include "post_process.h"


/** Utility function to compute the standard deviation in place.
 *  @param p the array we want to compute the standard deviation
 *  @param mean the mean of p
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 *  */
double stddev(const Eigen::VectorXf &p, double mean, bool unbiased=false) {
    float diff;
    float sum = 0;
    for(float i : p) {
        // compute difference
        diff = i - mean;
        sum += diff*diff;
    }
    if (unbiased) {
        // divide by N - 1
        sum /= p.size()-1;
    } else {
        // divide by N
        sum /= p.size();
    }
    // return square root
    return std::sqrt(sum);
}

/** Computes the KL divergence (p||q)
 *  @param p is the array of predicted values
 *  @param q is the array of alternative predicted values
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 */
double kl_divergence(const Eigen::VectorXf &p, const Eigen::VectorXf &q, bool unbiased) {
    // compute mean and standard deviation
    double mu_1 = p.mean();
    double sigma_1 = stddev(p, mu_1, unbiased);
    double mu_2 = q.mean();
    double sigma_2 = stddev(q, mu_2, unbiased);
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + ((sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2)) - 0.5;
    return kl;
}

/**
 * Functions that prints all plots, including computing the KL divergence
 * @param d: CleanedDataset object
 * @param plotter: PlotML object
 * @param true_preds: matrix containing the predictions on the original dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param alt_preds: vector of matrices, each relative to a different alternated dataset. Each matrix contains the predictions on the alternated dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param label_name: the name of the evaluated PBA
 */
void process_results(const CleanedDataset& d, const PlotML& plotter, Eigen::MatrixXf& true_preds, vector<Eigen::MatrixXf>& alt_preds, const string& label_name){

    int attribute_num_categories = 2; //TODO replace with actual categories from dataset
    vector<string> categories{"male", "female"}; //TODO replace with actual categories from dataset


    for(int a1 = 0, i = 0; a1 < attribute_num_categories-1 ; a1++)
        for(int a2 = a1+1; a2 < attribute_num_categories ; a2++, i++){ // for each

            const string attr1_name = categories[a1];
            const string attr2_name = categories[a2];

            plotter.alternation_plot(attr1_name, attr2_name, label_name, true_preds.row(a1), alt_preds[i].row(a2));

            // get indices of next attribute pair
            if (++a2 >= attribute_num_categories) {
                a2 = ++a1 + 1;
            }
        }
}