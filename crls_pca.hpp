// Definitions of functions used in CRLS PCA algorithm.
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_CRLS_PCA_HPP
#define ITO_CRLS_PCA_HPP

#include <vector>
#include "pca.hpp"

using std::vector;

/**
 * Compute a CRLS PCA of a set of vectors
 * @param m number of principal components to compute
 * @param xs set of vector samples
 * @param MAX_EPOCHS maximal number of epochs per component
 * @param eps epsilon parameter of an algorithm
 * @return a pc structure (mean, weights and transformed data).
 */
pc crls_pca(uint32_t m, vector<vector<double>> xs, uint32_t MAX_EPOCHS, double eps);

#endif //ITO_CRLS_PCA_HPP
