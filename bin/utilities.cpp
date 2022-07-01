//
// Created by ilapi on 30/06/2022.
//

#include <iostream>
#include <Eigen/core>

using namespace Eigen;

int main()
{
    MatrixXd m(2,2);
    m(0,0) = 3;
    m(1,0) = 2.5;
    m(0,1) = -1;
    m(1,1) = m(1,0) + m(0,1);
    std::cout << m << std::endl;
}

/** Computes the KL divergence (p||q)
 *  @param p is the array of predicted values
 *  @param q is the array of alternative predicted values
 */
double kl_divergence(const ArrayXf& p, const ArrayXf& q) {
    // compute mean and standard deviation
    double mu_1 = p.mean();
    double sigma_1 = std::sqrt((p - mu_1).square().sum()/(p.size()-1));
    double mu_2 = q.mean();
    double sigma_2 = std::sqrt((q - mu_2).square().sum()/(q.size()-1));
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + (sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2) - 0.5;
    return kl;
}

/** A wrapper for mlpack logistic regression train and predict functions.
 *  It returns an arma::vec array of predicted values.
 *  @param train is the training dataset
 *  @param responses is the known value (one for each entry of the dataset)
 *  @param samples is the dataset we want to evaluate
*/
/*vec lr_train_and_predict(const mat& train, const vec& responses, const mat& samples) {
    // generate the model
    LinearRegression lr;
    // get the parameters of such model
    vec parameters = lr.train(train, responses);
    // this will store the predictions, one row for each sample
    rowvec predictions;
    // predict the values
    lr.predict(samples, predictions);
    return predictions;
}
*/