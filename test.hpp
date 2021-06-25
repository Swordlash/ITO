//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_TEST_HPP
#define ITO_TEST_HPP

#include <functional>

void run_test(int, const std::function<void()>&);
void test_case_encode_decode(std::function<uint8_t()>&);
void test_case_crls_pca(std::function<uint8_t()>&);
bool imgs_equal(const png::image<png::rgb_pixel>&, const png::image<png::rgb_pixel>&);
png::image<png::rgb_pixel> rand_image(std::function<uint8_t()>&);

#endif //ITO_TEST_HPP
