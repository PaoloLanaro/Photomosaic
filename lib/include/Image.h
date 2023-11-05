//
// Created by planaro on 10/4/2023.
//

#ifndef IMAGELIB_IMAGE_H
#define IMAGELIB_IMAGE_H

#include <random>
#include "Pixel.h"

class Image {
private:
    std::string inputFilename{};
    std::string outputFilename{};
    Pixel **imageAsPixels{};
    int imageWidth{};
    int imageHeight{};
    int components{};
    unsigned char *data{};

    unsigned char *flattenImage();

    void deallocateArrayHelper(Pixel **&imageToDealloc) const;

    void setImageAsPixels(unsigned char *imageData);

public:
    // Constructors
    Image(const std::string& inputFile, const std::string& outputFile);
    explicit Image(const std::string& inputFile);
    Image();

    // ====Rule of Three====
    // Destructor
    ~Image();
    // Copy constructor
    Image(const Image &);
    // Copy Assignment Operator
    Image &operator=(const Image &other);


    enum class FileTypes {
        PNG,
        BMP,
        JPG,
        TGA,
    };

    void readImage();

    void writeImage(const FileTypes &);

    void writeImage(const std::string& outputFile, const FileTypes& fileType);

    void verticalAxisFlip();

    void horizontalAxisFlip();

    void rotateImage(bool);

    void addColoredBorder(unsigned int red, unsigned int green, unsigned int blue, unsigned int borderSize);

    void pointillism();

    Pixel getAverageImagePixel(const int& x, const int& y, const int& sideLength);
    Pixel getAverageImagePixel();

    void resizeImage(const int& widthToResize);
    void resizeImage(const int& widthToResize, const int& heightToResize);

    void clockwiseHelper(bool clockwise, Pixel **rotatedImage) const;

    void deepCopyHelper(Pixel **rotatedImage);

    void applyColorInSphere(int randRow, int randCol, int radius, unsigned int redColor, unsigned int greenColor,
                            unsigned int blueColor, unsigned int alphaColor);

    void
    saveRGBColors(int randRow, int randCol, unsigned int &redColor, unsigned int &greenColor, unsigned int &blueColor,
                  unsigned int &alphaColor) const;

    void pointillismHelper(int numPoints);

    void twoDArraySetHelper(const Image &other);

    // Allows the insertion of a separate image into this image. The replacing image's top left location will be the
    // x and y, but y is 0 indexed from the top, not a regular cartesian coordinate grid.
    void insertImage(const int& x, const int& y, const Image& image);

    // allows Photomosaic to access Image.imageWidth & Image.imageHeight so that I don't have to have getters.
    friend class Photomosaic;
};

#endif //IMAGELIB_IMAGE_H
