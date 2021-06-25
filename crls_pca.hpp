//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_CRLS_PCA_HPP
#define ITO_CRLS_PCA_HPP

#include <vector>

using std::vector;

constexpr uint32_t MAX_EPOCHS = 100;
constexpr double eps = 5e-9;

struct pc
{
    vector<vector<double>> weights;
    vector<double> mean;
    vector<vector<double>> transformed;
};

pc crls_pca(uint32_t, vector<vector<double>>);
vector<double> operator+(const vector<double>& u, const vector<double>& v);
vector<double> operator*(const vector<vector<double>>& w, const vector<double>& v);

#endif //ITO_CRLS_PCA_HPP
