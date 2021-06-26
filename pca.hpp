//
// Created by Mateusz Goslinowski on 25/06/2021.
//

#ifndef ITO_PCA_HPP
#define ITO_PCA_HPP

#include <vector>

using std::vector;

struct pc
{
    vector<vector<double>> weights;
    vector<double> mean;
    vector<vector<double>> transformed;
};

vector<double> operator+(const vector<double>&, const vector<double>&);
vector<double> operator*(const vector<vector<double>>&, const vector<double>&);

#endif //ITO_PCA_HPP
