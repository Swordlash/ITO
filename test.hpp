// Test utilities.
// Created by Mateusz Goslinowski on 16/06/2021.
//

#ifndef ITO_TEST_HPP
#define ITO_TEST_HPP

#include <png.hpp>
#include <functional>

using png::image;
using png::rgb_pixel;

/**
 * Runs a test a specific number of times.
 * @param reps
 * @param test
 */
void run_test(int reps, const std::function<void()>& test);

void test_case_encode_decode(std::function<uint8_t()>& rand);

/**
 * Checks whether two images are equal pixel-wise.
 * @param img1
 * @param img2
 * @return img1 == img2
 */
bool imgs_equal(const image<rgb_pixel>& img1, const image<rgb_pixel>& img2);

/**
 * Creates a random image
 * @param rand random number generator
 * @return a random image generated with a generator rand
 */
image<rgb_pixel> rand_image(std::function<uint8_t()>& rand);

/**
 * Calculates a peak-signal-to-noise ratio.
 * @param img1
 * @param img2
 * @return PSNR(img1, img2)
 */
double psnr(const image<rgb_pixel>& img1, const image<rgb_pixel>& img2);

#endif //ITO_TEST_HPP
