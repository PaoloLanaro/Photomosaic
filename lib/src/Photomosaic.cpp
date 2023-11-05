//
// Created by lanar on 10/26/2023.
//

#include <string>
#include "Photomosaic.h"
#include "Image.h"
#include "AVLMap.h"
#include <filesystem>

namespace fs = std::filesystem;
using std::string;

void Photomosaic::writeImage(const Image::FileTypes& fileType = Image::FileTypes::PNG) {
    srcImage.writeImage(outputImagePath, fileType);
}

void Photomosaic::createMosaic() {
    for (int row = 0; row < srcImage.imageWidth; row += imageSquareSizes) {
        for (int col = 0; col < srcImage.imageHeight; col += imageSquareSizes) {
            int xPos = row; // left - right position.
            int yPos = col; // up - down position
            Pixel avgPixel = srcImage.getAverageImagePixel(xPos, yPos, imageSquareSizes);
            Image closestImage = imageMap.search(avgPixel);
            srcImage.insertImage(xPos, yPos, closestImage);
        }
    }
}

void Photomosaic::loadImageLibrary() {
    fs::path imageLibrary(imageDirPath);

    AVLMap<string, string> validExtensions;
    validExtensions.insert(".jpg", ".jpg");
    validExtensions.insert(".jpeg", ".jpeg");
    validExtensions.insert(".png", ".png");
    validExtensions.insert(".bmp", ".bmp");
    validExtensions.insert(".tga", ".tga");
    for (const auto &entry: fs::recursive_directory_iterator(imageLibrary)) {
        const auto &filePath = entry.path();
        auto extension = filePath.extension().string();

        if (validExtensions.search(extension) != extension) {
            continue;
        }

        Image currentImage(filePath.relative_path());
        currentImage.readImage();
        currentImage.resizeImage(imageSquareSizes);
        Pixel pixelAvg = currentImage.getAverageImagePixel();
        imageMap.insert(pixelAvg, currentImage);
    }
}

Photomosaic::Photomosaic(const std::string &inputImagePath, const AVLMap<Pixel, Image>& imageDatabank,
                         const std::string &outputImagePath, const int &imageSquareSizes) {
//    this->imageDirPath = imageDirPath;
    this->outputImagePath = outputImagePath;
    this->imageSquareSizes = imageSquareSizes;

    this->imageMap = imageDatabank;

//    loadImageLibrary();

    Image srcImageRead(inputImagePath);
    srcImageRead.readImage();

    this->srcImage = srcImageRead;
}

Photomosaic::Photomosaic(const Image &inputImage, const AVLMap<Pixel, Image>& imageDatabank, const string &outputImagePath,
                         const int &imageSquareSizes) {
    this->srcImage = inputImage;
//    this->imageDirPath = imageDirPath;
    this->outputImagePath = outputImagePath;
    this->imageSquareSizes = imageSquareSizes;
    this->imageMap = imageDatabank;

    // initialize the map
//    loadImageLibrary();
}
