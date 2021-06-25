//
// Created by Mateusz Goslinowski on 15/06/2021.
//

#ifndef ITO_IMAGE_HPP
#define ITO_IMAGE_HPP

#include <png.hpp>
#include "crls_pca.hpp"

using img = png::image<png::rgb_pixel>;

struct encoded_image
{
    uint32_t block_size;
    uint32_t image_width;
    uint32_t image_height;
    std::vector<std::vector<double>> red_blocks;
    std::vector<std::vector<double>> green_blocks;
    std::vector<std::vector<double>> blue_blocks;

    encoded_image() = default;
    explicit encoded_image(uint32_t block_size, img& img);
    explicit operator img() const;

    encoded_image crls_pca(uint32_t components) const;
};



#endif //ITO_IMAGE_HPP
