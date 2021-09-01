//
// Created by Mateusz Goslinowski on 15/06/2021.
//

#include "logging.hpp"
#include "image.hpp"
#include "svd_pca.hpp"
#include "crls_pca.hpp"
#include "utils.hpp"
#include <cmath>
#include <execution>

using namespace std::placeholders;

encoded_image::encoded_image(uint32_t block_size, img &img) {
    log("Encoding PNG image. Block size: %d.", block_size);

    std::vector<std::vector<double>> red, green, blue;
    std::vector<std::vector<double>> created(3);

    for(uint32_t i=0; i<img.get_height(); i+=block_size) {
        for(uint32_t j=0; j<img.get_width(); j+=block_size) {
            for(uint32_t h=0; h<block_size; ++h) {
                for(uint32_t w=0; w<block_size; ++w) {
                    if(i+h >= img.get_height() || j+w >= img.get_width())
                        for(int k=0; k<3; ++k)
                            created[k].push_back(0);

                    else {
                        created[0].push_back(img[i+h][j+w].red);
                        created[1].push_back(img[i+h][j+w].green);
                        created[2].push_back(img[i+h][j+w].blue);
                    }
                }
            }


            red.emplace_back(std::move(created[0]));
            green.emplace_back(std::move(created[1]));
            blue.emplace_back(std::move(created[2]));

            created[0].clear();
            created[1].clear();
            created[2].clear();
        }
    }

    this->block_size = block_size;
    this->red_blocks = red;
    this->green_blocks = green;
    this->blue_blocks = blue;
    this->image_width = img.get_width();
    this->image_height = img.get_height();

    log("Encoded %d blocks", red.size());
}


encoded_image::operator img() const {
    log("Decoding to image");
    img image(image_width, image_height);


    uint32_t block_idx = 0;
    for(uint32_t i=0; i<image_height; i+=block_size) {
        for(uint32_t j=0; j<image_width; j+=block_size) {
            uint32_t idx_in_block = 0;

            for(uint32_t h = 0; h < block_size; ++h) {
                for(uint32_t w = 0; w < block_size; ++w) {
                    if(j + w < image_width && i + h < image_height) {
                        image[i+h][j+w].red = (uint8_t) std::round(red_blocks[block_idx][idx_in_block]);
                        image[i+h][j+w].green = (uint8_t) std::round(green_blocks[block_idx][idx_in_block]);
                        image[i+h][j+w].blue = (uint8_t) std::round(blue_blocks[block_idx][idx_in_block]);
                    }

                    ++ idx_in_block;
                }
            }
            ++block_idx;
        }
    }

    log("Decoded % blocks.", red_blocks.size());
    return image;
}


encoded_image encoded_image::crls_pca(uint32_t components, uint32_t MAX_EPOCHS, double eps) const {
    return this->general_pca(std::bind(::crls_pca, components, _1, MAX_EPOCHS, eps));
}

encoded_image encoded_image::svd_pca(uint32_t components) const {
    return this->general_pca(std::bind(::svd_pca, components, _1));
}

encoded_image encoded_image::general_pca(const std::function<pc(std::vector<std::vector<double>>)>& pca_function) const {
    log("Requesting SVD PCA encode-decode on image");

    log("Calculating red component PCA");
    auto encoded_red = pca_function(red_blocks);

    log("Calculating green component PCA");
    auto encoded_green = pca_function(green_blocks);

    log("Calculating blue component PCA");
    auto encoded_blue = pca_function(blue_blocks);

    auto input_size = matrix_size(red_blocks) + matrix_size(green_blocks) + matrix_size(blue_blocks);
    auto weights = matrix_size(encoded_red.weights) + matrix_size(encoded_green.weights) + matrix_size(encoded_blue.weights);
    auto output_size = matrix_size(encoded_red.transformed) + matrix_size(encoded_green.transformed) + matrix_size(encoded_blue.transformed);

    log("Finished. Input size (bytes): %d, output size (bytes, with weights): %d",
        input_size * sizeof(double),
        (weights + output_size)*sizeof(double));

    std::vector<std::vector<double>> red(encoded_red.transformed.size());
    std::transform(std::execution::par_unseq, encoded_red.transformed.begin(), encoded_red.transformed.end(), red.begin(), [&](auto& v) {
        return encoded_red.mean + transpose(encoded_red.weights) * v;
    });

    std::vector<std::vector<double>> green(encoded_green.transformed.size());
    std::transform(std::execution::par_unseq, encoded_green.transformed.begin(), encoded_green.transformed.end(), green.begin(), [&](auto& v) {
        return encoded_green.mean + transpose(encoded_green.weights) * v;
    });

    std::vector<std::vector<double>> blue(encoded_blue.transformed.size());
    std::transform(std::execution::par_unseq, encoded_blue.transformed.begin(), encoded_blue.transformed.end(), blue.begin(), [&](auto& v) {
        return encoded_blue.mean + transpose(encoded_blue.weights) * v;
    });

    encoded_image img;
    img.block_size = block_size;
    img.image_width = image_width;
    img.image_height = image_height;
    img.red_blocks = std::move(red);
    img.green_blocks = std::move(green);
    img.blue_blocks = std::move(blue);

    return img;
}


