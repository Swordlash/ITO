//
// Created by Mateusz Goslinowski on 30/08/2021.
//

#include "svd_pca.hpp"

#include "pca.hpp"
#include "logging.hpp"
#include "utils.hpp"
#include <vector>
#include <functional>
#include <numeric>
#include <execution>

#include "Eigen/Dense"
#include "Eigen/SVD"

using namespace std;
using namespace Eigen;

pc svd_pca(uint32_t m, vector<vector<double>> xs) {
    uint32_t N = xs.size();
    log ("Calculating SVD PCA for %d samples.", xs.size());

    // remove mean
    auto xs_mean = mean(xs);
    std::transform(std::execution::par_unseq, xs.begin(), xs.end(), xs.begin(), [&](auto& x) {
        return x - xs_mean;
    });

    MatrixXd X(xs.front().size(), xs.size());
    // initialize
    for(int i=0; i<xs.size(); ++i)
        for(int j=0; j<xs[i].size(); ++j)
            X(j,i) = xs[i][j];

    MatrixXd corr = X * X.transpose() / (double) N;

    auto svd = corr.bdcSvd(ComputeFullU);
    MatrixXd eigenvectors = svd.matrixU();

    log("SVD-PCA finished, computing transformation");

    vector<vector<double>> w(m);
    for(int i=0; i<m; ++i)
    {
        w[i].resize(xs.front().size());
        for(int j=0; j<w[i].size(); ++j)
            w[i][j] = eigenvectors(i,j);
    }

    pc res;
    res.mean = xs_mean;
    res.weights = w;
    res.transformed.resize(xs.size());
    std::transform(std::execution::par_unseq,
                   xs.begin(),
                   xs.end(),
                   res.transformed.begin(),
                   [&w] (auto& v) {
                       return w*v;
                   });

    log("Truncated eigenvalues:\n%s", print_matrix(w).c_str());

    return res;
}

