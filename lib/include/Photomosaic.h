//
// Created by lanar on 10/26/2023.
//

#ifndef PHOTOMOSAIC_PHOTOMOSAIC_H
#define PHOTOMOSAIC_PHOTOMOSAIC_H
#include "Image.h"
#include "AVLMap.h"

class Photomosaic {

private:
    Image srcImage{};
    std::string imageDirPath;
    std::string outputImagePath;
    AVLMap<Pixel, Image> imageMap;
    int imageSquareSizes;

    // helper for initial loading of the image library.
    // resizes images, then maps avg pixel to image
    void loadImageLibrary();

public:
    // constructor with everything as paths
    Photomosaic(const std::string& inputImagePath, const AVLMap<Pixel, Image>& imageDatabank,
                const std::string& outputImagePath, const int& imageSquareSizes);
    // constructor with src image as Image
    Photomosaic(const Image& inputImage, const AVLMap<Pixel, Image>& imageDatabank,
                const std::string& outputImagePath, const int& imageSquareSizes);

    // actually creates the mosaic internally
    void createMosaic();

    // write the image to specified filetype
    void writeImage(const Image::FileTypes& fileTypes);
};

#endif //PHOTOMOSAIC_PHOTOMOSAIC_H
