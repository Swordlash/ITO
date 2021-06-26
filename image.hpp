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

    /**
     * Encode image into square blocks block_size x block_size
     * @param block_size length of the block side
     * @param img image to be processed
     */
    explicit encoded_image(uint32_t block_size, img& img);

    /**
     * Decode encoded image back to PNG img structure.
     * @return PNG image represented by encoded blocks.
     */
    explicit operator img() const;

    /**
     * Compute CRLS PCA algorithm for each of the colour components and transform back to the encoded_image
     * structure.
     * @param components number of components to compute
     * @param MAX_EPOCHS max. number of epochs per component
     * @param eps epsilon value of an algorithm
     * @return Compressed-decompressed image
     */
    encoded_image crls_pca(uint32_t components, uint32_t MAX_EPOCHS, double eps) const;
};



#endif //ITO_IMAGE_HPP
