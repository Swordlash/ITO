//
// Created by Mateusz Goslinowski on 26/06/2021.
//
#include "test.hpp"
#include "png.hpp"
#include <numeric>
#include <execution>
#include <cmath>

using png::image;
using png::rgb_pixel;

double psnr(const image<rgb_pixel> &x, const image<rgb_pixel> &y) {
    if(x.get_width() != y.get_width() || x.get_height() != y.get_height())
        throw std::invalid_argument("Images have different dimensions");

    double MAX = 255;

    double MSE = 0;
    for(int j=0; j<x.get_height(); ++j) {
        MSE += std::transform_reduce(std::execution::par_unseq,
                                     x[j].begin(), x[j].end(), y[j].begin(), 0.0,
                                     std::plus<>(), [](auto& px, auto& py) {
                    return std::pow(px.red - py.red, 2) + std::pow(px.green - py.green, 2) + std::pow(px.blue - py.blue, 2);
                });
    }

    MSE /= 3.0 * x.get_width() * x.get_height();

    return 10.0 * log10(MAX*MAX / MSE);
}

bool imgs_equal(const image<rgb_pixel>& img1, const image<rgb_pixel>& img2) {
    if(img1.get_height() != img2.get_height()) return false;
    if(img2.get_width() != img2.get_width()) return false;


    for(int j=0; j<img1.get_height(); ++j)
        for(int i=0; i<img1[j].size(); ++i)
            if(img1[j][i].red != img2[j][i].red ||
               img1[j][i].green != img2[j][i].green ||
               img1[j][i].blue != img2[j][i].blue)
                return false;

    return true;
}