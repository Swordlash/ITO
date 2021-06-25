//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#include "crls_pca.hpp"
#include <numeric>
#include <functional>
#include <execution>

using std::vector;

double dot(const vector<double>&, const vector<double>&);
double dist_sq(const vector<double>&, const vector<double>&);

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

vector<double> operator-(const vector<double>& u, const vector<double>& v)
{
    vector<double> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::minus<>());

    return res;
}

vector<double> operator+(const vector<double>& u, const vector<double>& v)
{
    vector<double> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::plus<>());

    return res;
}

double dist_sq(const vector<double>& u, const vector<double>& v) {
    return dot(u-v, u-v);
}

double dot(const vector<double>& u, const vector<double>& v) {
    return std::transform_reduce(std::execution::par_unseq, u.begin(), u.end(), v.begin(), 0.0);
}

vector<double> operator*(double x, const vector<double>& v) {
    using std::placeholders::_1;
    vector<double> res(v.size());

    std::transform(std::execution::par_unseq, v.begin(), v.end(), res.begin(), std::bind(std::multiplies<>(), _1, x));
    return res;
}

vector<double> operator*(const vector<vector<double>>& w, const vector<double>& v) {
    vector<double> res(w.size());

    std::transform(std::execution::par_unseq, w.begin(), w.end(), res.begin(), [&](auto& wv) {
        return std::transform_reduce(std::execution::par_unseq, wv.begin(), wv.end(), v.begin(), 0.0);
    });

    return res;
}

vector<double> operator/(const vector<double>& v, double x) {
    using std::placeholders::_1;
    vector<double> res(v.size());

    std::transform(std::execution::par_unseq, v.begin(), v.end(), res.begin(), std::bind(std::divides<>(), _1, x));
    return res;
}

vector<double> mean(const vector<vector<double>>& xs) {
    vector<double> zero(xs.front().size(), 0.0);
    return std::accumulate(xs.begin(), xs.end(), zero, [](auto&& x, auto&& y) {
        return x+y;
    }) / (double) xs.size();
}


pc crls_pca(uint32_t m, vector<vector<double>> xs) {
    uint32_t N = xs.size();

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
        ws[j].resize(N+1);
        ws[j][0].resize(xs[0].size(), 1.0);

        etas[j].resize(N+1, 0.0);
        etas[j][0] = length_square_2d(errors[j-1]) / (double) N;

        ys[j].resize(N+1, 0.0);
        errors[j].resize(N+1);

        for(uint32_t s=1; s<=MAX_EPOCHS; ++s) {
            for(int k=1; k<=N; ++k) {
                ys[j][k] = dot(ws[j][k-1], errors[j-1][k]);
                etas[j][k] = ys[j][k]*ys[j][k] + etas[j][k-1];

                ws[j][k] = ws[j][k-1] + (ys[j][k]/etas[j][k]) * (errors[j-1][k] - (ys[j][k] * ws[j][k-1]));

                if(dist_sq(ws[j][k-1], ws[j][k]) < eps*eps) {
                    w[j] = ws[j][k];
                    goto stable;
                }
            }
        }

        stable:
        for(int k=1; k<=N; ++k) {
            ys[j][k] = dot(w[j], errors[j-1][k]);
            errors[j][k] = errors[j-1][k] - ys[j][k] * w[j];
        }
    }

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

    return res;
}
