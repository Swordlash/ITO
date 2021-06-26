// Definitions of functions used in CRLS PCA algorithm.
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_CRLS_PCA_HPP
#define ITO_CRLS_PCA_HPP

#include <vector>
#include "pca.hpp"

using std::vector;

pc crls_pca(uint32_t, vector<vector<double>>, uint32_t, double);

#endif //ITO_CRLS_PCA_HPP
