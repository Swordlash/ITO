//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#include "image.hpp"
#include "test.hpp"
#include <png.hpp>
#include <random>
#include <execution>

using png::image;
using png::rgb_pixel;

int main() {
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    std::function<uint8_t()> rand = std::bind(dist, gen);

    std::cout << "Testing image encoding/decoding" << std::endl;
    run_test(100, std::bind(test_case_encode_decode, rand));

    return 0;
}


image<rgb_pixel> rand_image(std::function<uint8_t()>& rand) {
    image<rgb_pixel> img(rand()+1, rand()+1);

    for(int i=0; i<img.get_width(); ++i)
        for(int j=0; j<img.get_height(); ++j)
        {
            img[j][i].red = rand();
            img[j][i].green = rand();
            img[j][i].blue = rand();
        }

    return img;
}

void run_test(int no_cases, const std::function<void()>& test_case) {
    int test_num;
    try {
        for(test_num = 1; test_num<=no_cases; ++test_num)
            test_case();
        std::cout << "OK, passed " << no_cases << " tests." << std::endl;
    }
    catch (std::logic_error& err) {
        std::cerr << "FAIL! (after " << test_num << " tests): " << err.what() << std::endl;
    }
}

void test_case_encode_decode(std::function<uint8_t()>& rand) {
    auto img = rand_image(rand);

    auto encoded = encoded_image(2+rand() % 100, img);
    auto decoded = static_cast<image<rgb_pixel>>(encoded);

    if(!imgs_equal(img, decoded))
        throw std::logic_error("Images not equal after encoding/decoding");
}

