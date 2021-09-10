// Numeric utilities.
// Created by Mateusz Goslinowski on 26/06/2021.
//

#ifndef ITO_UTILS_HPP
#define ITO_UTILS_HPP

#include <vector>
#include <string>
#include <execution>

using std::vector;

/**
 * Compute a dot product of two vectors
 * @param u first vector
 * @param v second vector
 * @return u * v
 */
template<typename T>
double dot(const vector<T>& u, const vector<T>& v) {
    return std::transform_reduce(std::execution::par_unseq, u.begin(), u.end(), v.begin(), 0.0);
}

/**
 * Compute a squared distance between two vectors.
 * @param u first vector
 * @param v second vector
 * @return |u-v|^2
 */
template <typename T>
double dist_sq(const vector<T>& u, const vector<T>& v) {
    return dot(u-v, u-v);
}

/**
 * Compute component-wise sum of two vectors
 * @param u first vector
 * @param v second vector
 * @return u + v
 */
template <typename T>
vector<T> operator+(const vector<T>& u, const vector<T>& v) {
    vector<T> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::plus<>());

    return res;
}

/**
 * Compute component-wise difference of two vectors
 * @param u first vector
 * @param v second vector
 * @return u - v
 */
template <typename T>
vector<T> operator-(const vector<T>& u, const vector<T>& v) {
    vector<T> res(u.size());
    std::transform(std::execution::par_unseq, u.begin(), u.end(), v.begin(), res.begin(), std::minus<>());

    return res;
}

/**
 * Multiply vector by a matrix (from the left)
 * @param w matrix factor
 * @param v vector factor
 * @return w * v
 */
template <typename T>
vector<T> operator*(const vector<vector<T>>& w, const vector<T>& v) {
    vector<T> res(w.size());

    std::transform(std::execution::par_unseq, w.begin(), w.end(), res.begin(), [&](auto& wv) {
        return std::transform_reduce(std::execution::par_unseq, wv.begin(), wv.end(), v.begin(), 0.0);
    });

    return res;
}

/**
 * Transpose a matrix
 * @param matrix
 * @return transposed matrix
 */
template <typename T>
std::vector<std::vector<T>> transpose (const std::vector<std::vector<T>>& matrix) {
    std::vector<std::vector<T>> res (matrix.front().size());
    for(int i=0; i<res.size(); ++i) {
        res[i].resize(matrix.size());
        for(int j=0; j<matrix.size(); ++j)
            res[i][j] = matrix[j][i];
    }

    return res;
}

/**
 * Multiply a vector by a scalar
 * @param x a scalar
 * @param v a vector
 * @return xv
 */
template <typename T>
vector<T> operator*(T x, const vector<T>& v) {
    using std::placeholders::_1;
    vector<T> res(v.size());

    std::transform(std::execution::par_unseq, v.begin(), v.end(), res.begin(), std::bind(std::multiplies<>(), _1, x));
    return res;
}

/**
 * Divide a vector by a scalar
 * @param v a vector
 * @param x a scalar
 * @return v/x
 */
template <typename T>
vector<T> operator/(const vector<T>& v, T x) {
    using std::placeholders::_1;
    vector<T> res(v.size());

    std::transform(std::execution::par_unseq, v.begin(), v.end(), res.begin(), std::bind(std::divides<>(), _1, x));
    return res;
}

/**
 * Compute a mean of a vector of vectors
 * @param xs set of vectors
 * @return a mean vector
 */
template <typename T>
vector<T> mean(const vector<vector<T>>& xs) {
    vector<T> zero(xs.front().size(), 0.0);
    return std::accumulate(xs.begin(), xs.end(), zero, [](auto&& x, auto&& y) {
        return x+y;
    }) / (T) xs.size();
}

/**
 * Calculate a size of a matrix (a number of entries)
 * @param w A matrix to measure
 * @return A number of entries in the matrix;
 */
template <typename T>
int matrix_size(const vector<vector<T>>& w) {
    return std::transform_reduce(std::execution::par_unseq,
                                 w.begin(), w.end(), 0,
                                 std::plus<>(), std::mem_fn(&vector<T>::size));
}


#endif //ITO_UTILS_HPP
