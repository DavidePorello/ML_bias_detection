//
// Created by ilapi on 30/06/2022.
//

// Includes all relevant components of mlpack.
#include <armadillo>
#include <iostream>
// Convenience.
//using namespace mlpack;
using namespace arma;
int main()
{
    /*
    // First, load the data.
    arma::mat data;
    // Use data::Load() which transposes the matrix.
    data::Load("data.csv", data, true);
    // Now compute the covariance.  We assume that the data is already centered.
    // Remember, because the matrix is column-major, the covariance operation is
    // transposed.
    arma::mat cov = data * trans(data) / data.n_cols;
    // Save the output.
    data::Save("cov.csv", cov, true);
     */
    vec a = linspace(0, 5, 6);
    std::cout << a;
}

/** Computes the KL divergence (p||q)
 *  @param p is the array of predicted values
 *  @param q is the array of alternative predicted values
 */
double kl_divergence(const vec& p, const vec& q) {
    // compute mean and variance
    double mu_1 = mean(p);
    double sigma_1 = var(p);
    double mu_2 = mean(q);
    double sigma_2 = var(q);
    double diff = mu_1 - mu_2;
    // now, compute the KL divergence
    double kl = log(sigma_2/sigma_1) + (sigma_1*sigma_1 + diff*diff)/(2*sigma_2*sigma_2) - 0.5;
    return kl;
}

