// Implementation of CRLS PCA algorithm.
// Created by Mateusz Goslinowski on 16/06/2021.
//

#include "crls_pca.hpp"
#include "logging.hpp"
#include "utils.hpp"
#include <numeric>
#include <functional>
#include <execution>

using std::vector;


double square(double x) {
    return x*x;
}

double length_square_2d(vector<vector<double>>& vs)
{
    return std::transform_reduce(std::execution::par_unseq, vs.begin(), vs.end(), 0.0,std::plus<>(), [&](auto& v)
    {
        return std::transform_reduce(std::execution::par_unseq, v.begin(), v.end(), 0.0, std::plus<>(), square);
    });
}


pc crls_pca(uint32_t m, vector<vector<double>> xs, uint32_t MAX_EPOCHS, double eps) {
    uint32_t N = xs.size();
    log ("Calculating CRLS PCA for %d samples. Max no of epochs: %d, eps: %lf", xs.size(), MAX_EPOCHS, eps);

    // remove mean
    auto xs_mean = mean(xs);
    std::transform(std::execution::par_unseq, xs.begin(), xs.end(), xs.begin(), [&](auto& x) {
       return x - xs_mean;
    });

    vector<vector<vector<double>>> errors(m+1);
    errors[0].resize(N+1);
    std::copy(xs.begin(), xs.end(), errors[0].begin() + 1); // 1

    vector<vector<vector<double>>> ws(m+1);
    vector<vector<double>> w(m+1);
    vector<vector<double>> etas (m+1);

    vector<double> y(N+1);
    vector<vector<double>> ys(m+1);

    for(int j=1; j<=m; ++j) {
        log("Calculating %d-th component", j);
        ws[j].resize(N+1);
        ws[j][0].resize(xs[0].size(), 1.0);

        etas[j].resize(N+1, 0.0);
        etas[j][0] = length_square_2d(errors[j-1]) / (double) N;
        log("Residual error after %d components: %lf", j-1, etas[j][0]);

        ys[j].resize(N+1, 0.0);
        errors[j].resize(N+1);

        for(uint32_t s=1; s<=MAX_EPOCHS; ++s) {
            for(int k=1; k<=N; ++k) {
                ys[j][k] = dot(ws[j][k-1], errors[j-1][k]);
                etas[j][k] = ys[j][k]*ys[j][k] + etas[j][k-1];

                ws[j][k] = ws[j][k-1] + (ys[j][k]/etas[j][k]) * (errors[j-1][k] - (ys[j][k] * ws[j][k-1]));

                double e;
                if((e = dist_sq(ws[j][k-1], ws[j][k])) < eps*eps) {
                    log("Finishing after %d epochs and %d iterations.", s, k);
                    w[j] = ws[j][k];
                    goto stable;
                }
                else if(k == N && s == MAX_EPOCHS) {
                    log("WARNING: finishing without reaching epsilon value. After %d epochs and %d iterations, error: %.12lf", s, k, e);
                    w[j] = ws[j][k];
                }
            }
        }

        if(w[j].empty())
            throw std::invalid_argument(
                    "Maximum number of epochs exceeded not reaching a given epsilon\nEnlarge the number of epochs or epsilon");

        stable:
        for(int k=1; k<=N; ++k) {
            ys[j][k] = dot(w[j], errors[j-1][k]);
            errors[j][k] = errors[j-1][k] - ys[j][k] * w[j];
        }
    }

    log("CRLS-PCA finished, computing transformation");

    w.erase(w.begin());

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

    auto input_size = std::transform_reduce(std::execution::par_unseq,
                                               xs.begin(), xs.end(), 0,
                                               std::plus<>(), std::mem_fn(&vector<double>::size));

    auto weights = std::transform_reduce(std::execution::par_unseq,
                                               w.begin(), w.end(), 0,
                                               std::plus<>(), std::mem_fn(&vector<double>::size));

    auto output_size = std::transform_reduce(std::execution::par_unseq,
                                               res.transformed.begin(), res.transformed.end(), 0,
                                               std::plus<>(), std::mem_fn(&vector<double>::size));

    log("Truncated eigenvalues:\n%s", print_matrix(w).c_str());

    log("Finished. Input size (bytes): %d, output size (bytes, with weights): %d",
        input_size * sizeof(double),
        (weights + output_size + N)*sizeof(double));

    return res;
}
