//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_TEST_HPP
#define ITO_TEST_HPP

#include <png.hpp>
#include <functional>

using png::image;
using png::rgb_pixel;

void run_test(int, const std::function<void()>&);
void test_case_encode_decode(std::function<uint8_t()>&);
void test_case_crls_pca(std::function<uint8_t()>&);

bool imgs_equal(const image<rgb_pixel>&, const image<rgb_pixel>&);

image<rgb_pixel> rand_image(std::function<uint8_t()>&);
double psnr(const image<rgb_pixel>&, const image<rgb_pixel>&);

#endif //ITO_TEST_HPP
