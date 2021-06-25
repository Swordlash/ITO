//
// Created by Mateusz Goslinowski on 16/06/2021.
//

#include "image.hpp"
#include "test.hpp"
#include <png.hpp>
#include <random>

using png::image;
using png::rgb_pixel;

int main() {
    std::random_device r;
    std::mt19937 gen(r());
    std::uniform_int_distribution<uint8_t> dist(0, 255);
    std::function<uint8_t()> rand = std::bind(dist, gen);

    std::cout << "Testing image encoding/decoding" << std::endl;
    run_test(10, std::bind(test_case_encode_decode, rand));

    std::cout << "Testing crls pca" << std::endl;
    run_test(1, std::bind(test_case_crls_pca, rand));

    return 0;
}


bool imgs_equal(const image<rgb_pixel>& img1, const image<rgb_pixel>& img2) {
    if(img1.get_height() != img2.get_height()) return false;
    if(img2.get_width() != img2.get_width()) return false;

    for(int i=0; i<img1.get_width(); ++i)
    {
        for(int j=0; j<img1.get_height(); ++j)
        {
            if(img1[j][i].red != img2[j][i].red
            || img1[j][i].green != img2[j][i].green
            || img1[j][i].blue != img2[j][i].blue) return false;
        }
    }
    return true;
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

    auto encoded = encoded_image(img.get_width()*img.get_height() / (rand() + 1) + 1, img);
    auto decoded = static_cast<image<rgb_pixel>>(encoded);

    if(!imgs_equal(img, decoded))
        throw std::logic_error("Images not equal after encoding/decoding");
}

void test_case_crls_pca(std::function<uint8_t()>& rand) {
    image<rgb_pixel> img("/Users/mateusz/ITO/build/trasa.png");

    auto encoded = encoded_image(3, img);

    auto decoded1 = static_cast<image<rgb_pixel>>(encoded);
    decoded1.write("/Users/mateusz/ITO/build/trasa2.png");

    auto enc_dec = encoded.crls_pca(9);

    image<rgb_pixel> decoded = static_cast<image<rgb_pixel>>(enc_dec);
    decoded.write("/Users/mateusz/ITO/build/trasa3.png");
}