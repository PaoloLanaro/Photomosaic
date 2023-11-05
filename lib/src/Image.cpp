//
// Created by planaro on 10/4/2023.
//

#include <stdexcept>
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"

#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb_image_resize2.h"

#include <iostream>
#include <string>
#include <random>

using std::string;

Image::Image(const std::string &inputFilename, const std::string &outputFilename) {
    this->inputFilename = inputFilename;
    this->outputFilename = outputFilename;
//    data = new unsigned char[imageHeight * imageWidth * components];
}

Image::Image(const string &inputFile) {
    this->inputFilename = inputFile;
}

Image::Image() {
    this->inputFilename = "";
    this->outputFilename = "";
}

Image::~Image() {
    deallocateArrayHelper(imageAsPixels);
    stbi_image_free(data);
}

void Image::readImage() {
    if (inputFilename.empty()) {
        throw std::runtime_error("No input file provided");
    }

    if (data != nullptr) {
        delete[] data;
        data = nullptr;
    }

    data = stbi_load(inputFilename.c_str(), &imageWidth, &imageHeight, &components, 0);
    if (!data) {
        throw std::runtime_error("Failed to load image.");
    }

    setImageAsPixels(data);
}

void Image::setImageAsPixels(unsigned char *imageData) {
    if (imageAsPixels != nullptr) {
        deallocateArrayHelper(imageAsPixels);
    }

    imageAsPixels = new Pixel *[imageHeight];
    for (int row = 0; row < imageHeight; row++) {
        imageAsPixels[row] = new Pixel[imageWidth];
        for (int col = 0; col < imageWidth; col++) {
            int index = (row * imageWidth + col) * components;
            if (components == 4) {
                Pixel pixel(imageData[index], imageData[index + 1], imageData[index + 2], imageData[index + 3]);
                imageAsPixels[row][col] = pixel;
            } else {
                Pixel pixel(imageData[index], imageData[index + 1], imageData[index + 2]);
                imageAsPixels[row][col] = pixel;
            }
        }
    }
}

unsigned char *Image::flattenImage() {
    auto *flattenedImage = new unsigned char[imageWidth * imageHeight * components];

    for (int row = 0; row < imageHeight; row++) {
        for (int col = 0; col < imageWidth; col++) {
            Pixel pixel = imageAsPixels[row][col];
            int index = (row * imageWidth + col) * components;
            flattenedImage[index] = static_cast<unsigned char>(pixel.getRedValue());
            flattenedImage[index + 1] = static_cast<unsigned char>(pixel.getGreenValue());
            flattenedImage[index + 2] = static_cast<unsigned char>(pixel.getBlueValue());
            if (components == 4) {
                flattenedImage[index + 3] = static_cast<unsigned char>(pixel.getAlphaValue());
            }
        }
    }
    return flattenedImage;
}

void Image::writeImage(const FileTypes &output = Image::FileTypes::PNG) {
    if (!imageAsPixels) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }

    if (outputFilename.empty()) {
        throw std::runtime_error("No output file specified");
    }

    // Create a char array of the size of elements in the image
    unsigned char *flattenedImage = flattenImage();

    // Write the image using stb_image_write
    if (output == FileTypes::PNG) {

        if (!stbi_write_png(outputFilename.c_str(), imageWidth, imageHeight, components, flattenedImage,
                            imageWidth * components)) {
            throw std::runtime_error("Failed to write image in PNG format.");
        }
    } else if (output == FileTypes::BMP) {
        if (!stbi_write_bmp(outputFilename.c_str(), imageWidth, imageHeight, components, flattenedImage)) {
            throw std::runtime_error("Failed to write image in BMP format.");
        }
    } else if (output == FileTypes::JPG) {
        if (!stbi_write_jpg(outputFilename.c_str(), imageWidth, imageHeight, components, flattenedImage,
                            imageWidth * sizeof(int))) {
            throw std::runtime_error("Failed to write image in JPG format.");
        }
    } else if (output == FileTypes::TGA) {
        if (!stbi_write_tga(outputFilename.c_str(), imageWidth, imageHeight, components, flattenedImage)) {
            throw std::runtime_error("Failed to write image in TGA format.");
        }
    }

    // Cleaning up flattenedImage
    delete[] flattenedImage;
}

void Image::verticalAxisFlip() {
    if (!imageAsPixels) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }

    // Flip the image along the vertical axis
    for (int row = 0; row < imageHeight; row++) {
        for (int col = 0; col < imageWidth / 2; col++) {
            std::swap(imageAsPixels[row][col], imageAsPixels[row][imageWidth - col - 1]);
        }
    }
}

void Image::horizontalAxisFlip() {
    if (!imageAsPixels) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }

    // Flip the image along the horizontal axis
    for (int row = 0; row < imageHeight / 2; row++) {
        for (int col = 0; col < imageWidth; col++) {
            std::swap(imageAsPixels[row][col], imageAsPixels[imageHeight - row - 1][col]);
        }
    }
}

void Image::rotateImage(bool clockwise) {
    if (!imageAsPixels) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }

    // Create a new temp data structure for the rotatedImage
    auto **rotatedImage = new Pixel *[imageWidth];
    for (int row = 0; row < imageWidth; row++) {
        rotatedImage[row] = new Pixel[imageHeight];
    }

    clockwiseHelper(clockwise, rotatedImage);

    // Clean up the old image data
    deallocateArrayHelper(imageAsPixels);

    // Update the image dimensions and data
    int tempWidth = imageWidth;
    imageWidth = imageHeight;
    imageHeight = tempWidth;

    // Create deep copy of rotatedImage for imageAsPixels
    deepCopyHelper(rotatedImage);
}

void Image::deepCopyHelper(Pixel **imageToDeepCopy) {
    imageAsPixels = new Pixel *[imageHeight];
    for (int row = 0; row < imageHeight; row++) {
        imageAsPixels[row] = new Pixel[imageWidth];
        for (int col = 0; col < imageWidth; col++) {
            imageAsPixels[row][col] = imageToDeepCopy[row][col];
        }
    }
    deallocateArrayHelper(imageToDeepCopy);
}

void Image::clockwiseHelper(bool clockwise, Pixel **rotatedImage) const {
    if (clockwise) {
        for (int row = 0; row < imageHeight; row++) {
            for (int col = 0; col < imageWidth; col++) {
                rotatedImage[col][imageHeight - row - 1] = imageAsPixels[row][col];
            }
        }
    } else {  // Counter-clockwise
        for (int row = 0; row < imageHeight; row++) {
            for (int col = 0; col < imageWidth; col++) {
                rotatedImage[imageWidth - col - 1][row] = imageAsPixels[row][col];
            }
        }
    }
}

// Clean up 2d image data
void Image::deallocateArrayHelper(Pixel **&imageToDealloc) const {
    if (imageToDealloc != nullptr) {
        for (int row = 0; row < imageHeight; row++) {
            if (imageToDealloc[row] != nullptr) {
                delete[] imageToDealloc[row];
                imageToDealloc[row] = nullptr; // Set the pointer to nullptr after deletion to avoid double-delete
            }
        }
        delete[] imageToDealloc;
        imageToDealloc = nullptr;
    } else {
        throw std::runtime_error("Cannot deallocate non-existent image");
    }
}

void Image::addColoredBorder(unsigned int red, unsigned int green, unsigned int blue, unsigned int borderSize) {
    // imageHeight += 2 * borderSize
    // imageWidth += 2* borderSize
    unsigned int totalImageHeight = imageHeight + (2 * borderSize);
    unsigned int totalImageWidth = imageWidth + (2 * borderSize);
    auto **temporaryImage = new Pixel *[totalImageHeight];
    for (int row = 0; row < totalImageHeight; row++) {
        temporaryImage[row] = new Pixel[totalImageWidth];
        for (int col = 0; col < totalImageWidth; col++) {
            if (row < borderSize || col < borderSize || row >= (totalImageHeight - borderSize) ||
                col >= (totalImageWidth - borderSize)) {
                temporaryImage[row][col].setRedValue(red);
                temporaryImage[row][col].setGreenValue(green);
                temporaryImage[row][col].setBlueValue(blue);
                temporaryImage[row][col].setAlpha(255);
            } else {
                temporaryImage[row][col] = imageAsPixels[row - borderSize][col - borderSize];
            }
        }
    }

    // Clean up old image
    deallocateArrayHelper(imageAsPixels);

    // Create deepcopy of temp image
    deepCopyHelper(temporaryImage);

    // Clean up temp image
    deallocateArrayHelper(temporaryImage);

    // Set new width and height of the image
    imageWidth = (int) totalImageWidth;
    imageHeight = (int) totalImageHeight;
}

void Image::pointillism() {
    if (!imageAsPixels) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }


    int numPoints = (5 * (imageHeight * imageWidth) / imageWidth) * 2;

    pointillismHelper(numPoints);
}

void Image::pointillismHelper(int numPoints) {
    std::random_device rd;
    std::mt19937 gen(rd());
    for (int i = 0; i < numPoints; i++) {
        // Randomly select a pixel
        std::uniform_int_distribution<int> randRowDist(0, imageHeight - 1);
        std::uniform_int_distribution<int> randColDist(0, imageWidth - 1);

        int randRow = randRowDist(gen);
        int randCol = randColDist(gen);

        // Randomly choose a sphere radius
        double scaleFactor = 0.0007 * (imageHeight * imageWidth) / 1000000.0;

        // Scale the radius range based on the scaling factor
        int minRadius = 5;
        int maxRadius = std::max(6, static_cast<int>(std::min(imageHeight, imageWidth) * scaleFactor));

        std::uniform_int_distribution<int> radiusDist(minRadius, maxRadius);
        int radius = radiusDist(gen);

        // Save pixel color to int values
        unsigned int redColor;
        unsigned int greenColor;
        unsigned int blueColor;
        unsigned int alphaColor;
        saveRGBColors(randRow, randCol, redColor, greenColor, blueColor, alphaColor);

        // Apply the color to the surrounding pixels within the sphere
        applyColorInSphere(randRow, randCol, radius, redColor, greenColor, blueColor, alphaColor);
    }
}

void Image::saveRGBColors(int randRow, int randCol, unsigned int &redColor, unsigned int &greenColor,
                          unsigned int &blueColor, unsigned int &alphaColor) const {
    redColor = imageAsPixels[randRow][randCol].getRedValue();
    greenColor = imageAsPixels[randRow][randCol].getGreenValue();
    blueColor = imageAsPixels[randRow][randCol].getBlueValue();
    if (components == 4) {
        alphaColor = imageAsPixels[randRow][randCol].getAlphaValue();
    }
}

void Image::applyColorInSphere(int randRow, int randCol, int radius, unsigned int redColor, unsigned int greenColor,
                               unsigned int blueColor, unsigned int alphaColor) {
    for (int row = -radius; row <= radius; row++) {
        for (int col = -radius; col <= radius; col++) {
            int targetRow = randRow + row;
            int targetCol = randCol + col;

            if (targetRow >= 0 && targetRow < imageHeight && targetCol >= 0 && targetCol < imageWidth) {
                int distanceSquared = row * row + col * col;
                int radiusSquared = radius * radius;

                // Check if the pixel is within the sphere
                if (distanceSquared <= radiusSquared) {
                    imageAsPixels[targetRow][targetCol].setRedValue(redColor);
                    imageAsPixels[targetRow][targetCol].setGreenValue(greenColor);
                    imageAsPixels[targetRow][targetCol].setBlueValue(blueColor);
                    if (components == 4) {
                        imageAsPixels[targetRow][targetCol].setAlpha(alphaColor);
                    }
                }
            }
        }
    }
}

// Copy constructor of the Image class
Image::Image(const Image &arg) {
    if (arg.imageAsPixels == nullptr) {
        throw std::runtime_error("Image data is not available. Call readImage() to load the image first.");
    }

    this->imageHeight = arg.imageHeight;
    this->imageWidth = arg.imageWidth;
    this->components = arg.components;

    this->inputFilename = arg.inputFilename;
    this->outputFilename = arg.outputFilename;

    if (arg.data) {
        int dataSize = arg.imageHeight * arg.imageWidth * arg.components;
        data = new unsigned char[dataSize];
        std::copy(arg.data, arg.data + dataSize, data);
    } else {
        this->data = nullptr;
    }

    twoDArraySetHelper(arg);

//    this->imageAsPixels = new Pixel *[imageHeight];
//    for (int row = 0; row < imageHeight; row++) {
//        this->imageAsPixels[row] = new Pixel[imageWidth];
//        for (int col = 0; col < imageWidth; col++) {
//            int index = (row * imageWidth + col) * components;
//            this->imageAsPixels[row][col].setRedValue(arg.data[index]);
//            this->imageAsPixels[row][col].setGreenValue(arg.data[index + 1]);
//            this->imageAsPixels[row][col].setBlueValue(arg.data[index + 2]);
//            this->imageAsPixels[row][col].setAlpha(arg.data[index + 3]);
//        }
//    }
}

Pixel Image::getAverageImagePixel() {

    unsigned int redAverage{};
    unsigned int greenAverage{};
    unsigned int blueAverage{};
    int pixelCount{};

    for (int row = 0; row < imageHeight; row++) {
        if (imageAsPixels[row] == nullptr) {
            throw std::runtime_error(
                    "Could not read average RGB of image's rows because the image's rows doesn't exist.");
        }
        for (int col = 0; col < imageWidth; col++) {
            Pixel currPixel = imageAsPixels[row][col];
            redAverage += currPixel.getRedValue();
            greenAverage += currPixel.getGreenValue();
            blueAverage += currPixel.getBlueValue();
            pixelCount++;
        }
    }
    redAverage = redAverage / pixelCount;
    greenAverage = greenAverage / pixelCount;
    blueAverage = blueAverage / pixelCount;
    return {redAverage, greenAverage, blueAverage};
}

Pixel Image::getAverageImagePixel(const int &x, const int &y, const int &sideLength) {
    if (x < 0 || y < 0) {
        throw std::runtime_error("The specified area is outside the valid range of indices for the image.");
    }

    if (sideLength < 0) {
        throw std::runtime_error("Can't get a negative sized chunk of the image.");
    }

    int correctedYSideLength = std::min(y + sideLength, imageHeight);
    int correctedXSideLength = std::min(x + sideLength, imageWidth);

    if (imageAsPixels == nullptr) {
        throw std::runtime_error("Could not read average RGB of image because the image doesn't exist.");
    }

    unsigned int redAverage{};
    unsigned int greenAverage{};
    unsigned int blueAverage{};
    int pixelCount{};

    // custom side lengths
    for (int row = y; row < correctedYSideLength; row++) {
        if (imageAsPixels[row] == nullptr) {
            throw std::runtime_error(
                    "Could not read average RGB of image's rows because the image's rows doesn't exist.");
        }
        for (int col = x; col < correctedXSideLength; col++) {
            Pixel currPixel = imageAsPixels[row][col];
            redAverage += currPixel.getRedValue();
            greenAverage += currPixel.getGreenValue();
            blueAverage += currPixel.getBlueValue();
            pixelCount++;
        }
    }
    redAverage = redAverage / pixelCount;
    greenAverage = greenAverage / pixelCount;
    blueAverage = blueAverage / pixelCount;
    return {redAverage, greenAverage, blueAverage};


//    for (int row = 0; row < imageHeight; row++) {
//        if (imageAsPixels[row] == nullptr) {
//            throw std::runtime_error(
//                    "Could not read average RGB of image's rows because the image's rows doesn't exist.");
//        }
//        for (int col = 0; col < imageWidth; col++) {
//            Pixel currPixel = imageAsPixels[row][col];
//            redAverage += currPixel.getRedValue();
//            greenAverage += currPixel.getGreenValue();
//            blueAverage += currPixel.getBlueValue();
//            pixelCount++;
//        }
//    }
//    redAverage = redAverage / pixelCount;
//    greenAverage = greenAverage / pixelCount;
//    blueAverage = blueAverage / pixelCount;
//    return {redAverage, greenAverage, blueAverage};
}

void Image::resizeImage(const int &widthToResize, const int &heightToResize) {
    auto *resizedImage = new unsigned char[widthToResize * heightToResize * components];

    stbir_resize_uint8_srgb(flattenImage(), imageWidth, imageHeight, 0, resizedImage,
                            widthToResize, heightToResize, 0, (stbir_pixel_layout) components);

    deallocateArrayHelper(imageAsPixels);

    imageWidth = widthToResize;
    imageHeight = heightToResize;

    setImageAsPixels(resizedImage);
    delete[] resizedImage;
}

void Image::resizeImage(const int &widthToResize) {
    resizeImage(widthToResize, widthToResize);
}

// copy assignment operator
Image &Image::operator=(const Image &other) {
    // Check for self-assignment
    if (this == &other) {
        return *this;
    }

    this->imageHeight = other.imageHeight;
    this->imageWidth = other.imageWidth;

    this->inputFilename = other.inputFilename;
    this->outputFilename = other.outputFilename;
    this->components = other.components;

    // Release resources in the current object
    if (this->imageAsPixels != nullptr) {
        deallocateArrayHelper(imageAsPixels);
    }
    if (this->data != nullptr) {
        delete[] this->data;
        this->data = nullptr;
    }

    // Copy data and properties from the 'other' object
    if (!other.imageAsPixels) {
        throw std::runtime_error("Image data is not available in the source object "
                                 "(Probably didn't read the original image yet). Input image: "
                                 + other.inputFilename);
    }

    twoDArraySetHelper(other);

    // Deep copy 'data' if it's available in the source object
    if (other.data) {
        int dataSize = other.imageHeight * other.imageWidth * other.components;
        this->data = new unsigned char[dataSize];
        std::copy(other.data, other.data + dataSize, this->data);
    } else {
        this->data = nullptr;
    }
    return *this;
}

void Image::twoDArraySetHelper(const Image &other) {
    imageAsPixels = new Pixel *[other.imageHeight];
    for (int row = 0; row < other.imageHeight; row++) {
        imageAsPixels[row] = new Pixel[other.imageWidth];
        for (int col = 0; col < other.imageWidth; col++) {
            Pixel currentPixel = other.imageAsPixels[row][col];
            imageAsPixels[row][col].setRedValue(currentPixel.getRedValue());
            imageAsPixels[row][col].setGreenValue(currentPixel.getGreenValue());
            imageAsPixels[row][col].setBlueValue(currentPixel.getBlueValue());
            imageAsPixels[row][col].setAlpha(currentPixel.getAlphaValue());
        }
    }
}

void Image::insertImage(const int &x, const int &y, const Image &otherImage) {
    if (x < 0 || y < 0 || x > imageWidth || y > imageHeight) {
        throw std::runtime_error("Can't insert an image outside the bounds of the current image.");
    }
    int otherWidth = otherImage.imageWidth;
    int otherHeight = otherImage.imageHeight;
    int maxHeight = std::min(y + otherHeight, imageHeight);
    int maxWidth = std::min(x + otherWidth, imageWidth);
    for (int row = y; row < maxHeight; row++) {
        for (int col = x; col < maxWidth; col++) {
            this->imageAsPixels[row][col] = otherImage.imageAsPixels[row - y][col - x];
        }
    }

}

void Image::writeImage(const string &outputFile, const Image::FileTypes &fileType = Image::FileTypes::PNG) {
    outputFilename = outputFile;
    writeImage(fileType);
}
