#include <iostream>
#include <png.hpp>
#include "image.hpp"
using namespace std;

int main(int argc, char* argv[]) {
    png::image<png::rgb_pixel> image("/Users/mateusz/Downloads/rozw.png");

    encoded_image enc(64, image);

    png::image<png::rgb_pixel> decoded = static_cast<png::image<png::rgb_pixel>>(enc);
    decoded.write("result.png");
    return 0;
}
