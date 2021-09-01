//
// Created by Mateusz Goslinowski on 30/08/2021.
//

#ifndef ITO_SVD_PCA_HPP
#define ITO_SVD_PCA_HPP

#include <vector>
#include "pca.hpp"

/**
 * Compute a PCA of a set of vectors using the SVD decomposition
 * @param m number of principal components to compute
 * @param xs set of vector samples
 * @param MAX_EPOCHS maximal number of epochs per component
 * @param eps epsilon parameter of an algorithm
 * @return a pc structure (mean, weights and transformed data).
 */
pc svd_pca(uint32_t m, vector<vector<double>> xs);


#endif //ITO_SVD_PCA_HPP
