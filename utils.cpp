//
// Created by Mateusz Goslinowski on 26/06/2021.
//

#include <vector>
#include "utils.hpp"
#include <execution>

using std::vector;


vector<double> operator+(const vector<double>& u, const vector<double>& v) {
    vector<double> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::plus<>());

    return res;
}

vector<double> operator-(const vector<double>& u, const vector<double>& v) {
    vector<double> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::minus<>());

    return res;
}

vector<double> operator*(const vector<vector<double>>& w, const vector<double>& v) {
    vector<double> res(w.size());

    std::transform(std::execution::par_unseq, w.begin(), w.end(), res.begin(), [&](auto& wv) {
        return std::transform_reduce(std::execution::par_unseq, wv.begin(), wv.end(), v.begin(), 0.0);
    });

    return res;
}

std::vector<std::vector<double>> transpose (const std::vector<std::vector<double>>& matrix) {
    std::vector<std::vector<double>> res (matrix.front().size());
    for(int i=0; i<res.size(); ++i) {
        res[i].resize(matrix.size());
        for(int j=0; j<matrix.size(); ++j)
            res[i][j] = matrix[j][i];
    }

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