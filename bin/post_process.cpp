#include "post_process.h"




/** Computes the KL divergence (p||q)
 *  @param mu_1 is the mean of the standard predictions
 *  @param sigma_1 is the standard deviation of standard predictions
 *  @param mu_2 is the mean of the alternated predictions
 *  @param sigma_2 is the standard deviation of alternated predictions
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 */
double kl_divergence(double mu_1, double sigma_1, double mu_2, double sigma_2) {
    if(sigma_1 == 0 || sigma_2 == 0)
        return 0;
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + ((sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2)) - 0.5;
    return kl;
}

/**
 * Functions that prints all plots, including computing the KL divergence
 * @param d: CleanedDataset object
 * @param plotter: PlotML object
 * @param true_means: matrix containing the means of the predictions on the original dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param true_stddevs: matrix containing the standard deviations of the the predictions on the original dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param alt_means: vector of matrices, each relative to a different alternated dataset. Each matrix contains the means of the predictions on the alternated dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param alt_stddevs: vector of matrices, each relative to a different alternated dataset. Each matrix contains the standard deviations of the predictions on the alternated dataset. Each row is relative to 1 categorical attribute (e.g. male), each row is relative to 1 fold
 * @param label_name: the name of predction label (wage)
 * @param attr_index: the index of the PBA attribute
 */
void process_results(const CleanedDataset& d, const PlotML& plotter, Eigen::MatrixXf& true_means, Eigen::MatrixXf& true_stddevs, vector<Eigen::MatrixXf>& alt_means, vector<Eigen::MatrixXf>& alt_stddevs, const string& label_name, int attr_index){

    int attribute_num_categories = d.getNumberOfValues(attr_index);
    vector<string> categories = d.getCategories(attr_index);
    int num_folds = true_means.cols();

    for(int a1 = 0, i = 0; a1 < attribute_num_categories-1 ; a1++)
        for(int a2 = a1+1; a2 < attribute_num_categories ; a2++, i++){ // for each

            const string cat1_name = categories[a1];
            const string cat2_name = categories[a2];

            // print alternation plos
            plotter.alternation_plot(cat1_name, cat2_name, label_name, true_means.row(a1), alt_means[i].row(a1));
            plotter.alternation_plot(cat2_name, cat1_name, label_name, true_means.row(a2), alt_means[i].row(a2));

            // print bias evaluation plots
            Eigen::VectorXf kl1(num_folds), kl2(num_folds);
            for(int j=0; j< num_folds; j++){ // for each fold, compute KL divergence for category1 (female->male) and category2 (male->female)
                kl1(j) = kl_divergence(true_means(a1, j), true_stddevs(a1,j), alt_means[i](a1,j), alt_stddevs[i](a1, j));
                kl2(j) = kl_divergence(true_means(a2, j), true_stddevs(a2,j), alt_means[i](a2,j), alt_stddevs[i](a2, j));
            }
            plotter.bias_evaluation_plot(cat1_name, cat2_name, kl1,kl2);

        }
}