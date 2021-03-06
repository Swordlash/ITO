// Definition of an encoded_image structure.
// Created by Mateusz Goslinowski on 15/06/2021.
//

#ifndef ITO_IMAGE_HPP
#define ITO_IMAGE_HPP

#include <png.hpp>
#include <functional>
#include "crls_pca.hpp"

using img = png::image<png::rgb_pixel>;

class abstract_image
{
    explicit virtual operator img() const = 0;
};

/**
 * Encoded image - a class representing a PNG image decoded to square blocks.
 * Can be compressed by one of the PCA algorithms and written back to the PNG format.
 */
class encoded_image : public abstract_image
{
private:
    uint32_t block_size;
    uint32_t image_width;
    uint32_t image_height;
    std::vector<std::vector<double>> red_blocks;
    std::vector<std::vector<double>> green_blocks;
    std::vector<std::vector<double>> blue_blocks;

public:
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
    explicit operator img() const override;

    /**
     * Compute CRLS PCA algorithm for each of the colour components and transform back to the encoded_image
     * structure.
     * @param components number of components to compute
     * @param MAX_EPOCHS max. number of epochs per component
     * @param eps epsilon value of an algorithm
     * @return Compressed-decompressed image
     */
    encoded_image crls_pca(uint32_t components, uint32_t MAX_EPOCHS, double eps) const;

    /**
     * Compute PCA algorithm (through SVD transformation) for each of the colour components and transform back to the encoded_image
     * structure.
     * @param components number of components to compute
     * @param MAX_EPOCHS max. number of epochs per component
     * @param eps epsilon value of an algorithm
     * @return Compressed-decompressed image
     */
    encoded_image svd_pca(uint32_t components) const;

    /**
     * General PCA compression-decompression function
     * @param pca_function function used to calculate principal components
     * @return Compressed-decompressed image
     */
    encoded_image general_pca(const std::function<pc(std::vector<std::vector<double>>)>& pca_function) const;
};



#endif //ITO_IMAGE_HPP
