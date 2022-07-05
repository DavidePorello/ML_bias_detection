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


void process_results(const CleanedDataset& d, const PlotML& plotter, vector<vector<float>>& true_preds, vector<vector<float>>& alt_preds, const string& label_name){

    int attribute_num_categories = 2; //TODO replace with actual categories from dataset
    vector<string> categories{"male", "female"}; //TODO replace with actual categories from dataset


    for(int a1 = 0, i = 0; a1 < attribute_num_categories-1 ; a1++)
        for(int a2 = a1+1; a2 < attribute_num_categories ; a2++, i++){ // for each

            const string attr1_name = categories[a1];
            const string attr2_name = categories[a2];
            const Eigen::Map<Eigen::VectorXf> original_predictions(true_preds[a1].data(), attribute_num_categories);
            const Eigen::Map<Eigen::VectorXf> alternated_predictions(alt_preds[a1].data(), attribute_num_categories);

            plotter.alternation_plot(attr1_name, attr2_name, label_name, original_predictions, alternated_predictions);

            // get indices of next attribute pair
            if (++a2 >= attribute_num_categories) {
                a2 = ++a1 + 1;
            }
        }
}