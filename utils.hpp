// Numeric utilities.
// Created by Mateusz Goslinowski on 26/06/2021.
//

#ifndef ITO_UTILS_HPP
#define ITO_UTILS_HPP

#include <vector>
#include <string>

using std::vector;

/**
 * Compute a dot product of two vectors
 * @param u first vector
 * @param v second vector
 * @return u * v
 */
double dot(const vector<double>& u, const vector<double>& v);

/**
 * Compute a squared distance between two vectors.
 * @param u first vector
 * @param v second vector
 * @return |u-v|^2
 */
double dist_sq(const vector<double>& u, const vector<double>& v);

/**
 * Compute component-wise sum of two vectors
 * @param u first vector
 * @param v second vector
 * @return u + v
 */
vector<double> operator+(const vector<double>& u, const vector<double>& v);

/**
 * Compute component-wise difference of two vectors
 * @param u first vector
 * @param v second vector
 * @return u - v
 */
vector<double> operator-(const vector<double>& u, const vector<double>& v);

/**
 * Multiply vector by a matrix (from the left)
 * @param w matrix factor
 * @param v vector factor
 * @return w * v
 */
vector<double> operator*(const vector<vector<double>>& w, const vector<double>& v);

/**
 * Transpose a matrix
 * @param matrix
 * @return transposed matrix
 */
std::vector<std::vector<double>> transpose (const std::vector<std::vector<double>>& matrix);

/**
 * Multiply a vector by a scalar
 * @param x a scalar
 * @param v a vector
 * @return xv
 */
vector<double> operator*(double x, const vector<double>& v);

/**
 * Divide a vector by a scalar
 * @param v a vector
 * @param x a scalar
 * @return v/x
 */
vector<double> operator/(const vector<double>& v, double x);

/**
 * Compute a mean of a vector of vectors
 * @param xs set of vectors
 * @return a mean vector
 */
vector<double> mean(const vector<vector<double>>& xs);

/**
 * Prints the matrix to the string.
 * @param w matrix to be printed
 */
std::string print_matrix(const vector<vector<double>>& w);

#endif //ITO_UTILS_HPP
