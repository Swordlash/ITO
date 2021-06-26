// Definition of Principal Components structure
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

#endif //ITO_PCA_HPP
