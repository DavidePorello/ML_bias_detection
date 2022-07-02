#include <Eigen/Core>
#include "ModelML.h"

using namespace Eigen;

/** Utility function to compute the standard deviation in place.
 *  @param p the array we want to compute the standard deviation
 *  @param mean the mean of p
 *  @param unbiased if true, we divide by N-1 instead of N to reduce bias
 *  (https://en.wikipedia.org/wiki/Unbiased_estimation_of_standard_deviation)
 *  */
double stddev(const VectorXf &p, double mean, bool unbiased=false) {
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
 */
double kl_divergence(const VectorXf &p, const VectorXf &q) {
    // compute mean and standard deviation
    double mu_1 = p.mean();
    double sigma_1 = stddev(p, mu_1);
    double mu_2 = q.mean();
    double sigma_2 = stddev(q, mu_2);
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + ((sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2)) - 0.5;
    return kl;
}

/** A wrapper for mlpack logistic regression train and predict functions.
 *  It returns an arma::vec array of predicted values.
 *  @param train is the training dataset
 *  @param responses is the known value (one for each entry of the dataset)
 *  @param samples is the dataset we want to evaluate
 *  @param model is the type of model we want to use
 *  @param predictions will store the predicted values, one for each sample
*/
void train_and_predict(const MatrixXf &train, const VectorXf &responses, const MatrixXf &samples,
                             ModelML &model, VectorXf &predictions) {
    // train the model
    model.fit(train, responses);
    // predict the values
    model.predict(samples, predictions);
}