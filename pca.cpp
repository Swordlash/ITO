//
// Created by Mateusz Goslinowski on 26/06/2021.
//

#include <vector>
#include "pca.hpp"
#include <execution>

using std::vector;

vector<double> operator+(const vector<double>& u, const vector<double>& v)
{
    vector<double> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::plus<>());

    return res;
}

vector<double> operator*(const vector<vector<double>>& w, const vector<double>& v) {
    vector<double> res(w.size());

    std::transform(std::execution::par_unseq, w.begin(), w.end(), res.begin(), [&](auto& wv) {
        return std::transform_reduce(std::execution::par_unseq, wv.begin(), wv.end(), v.begin(), 0.0);
    });

    return res;
}