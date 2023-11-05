#include <iostream>
#include <string>

#include "Image.h"
#include "Photomosaic.h"

using std::cout, std::endl, std::cin, std::string;

void consoleUser();

void testExample();

void commandLineUser(const char* argv);

int main(int argc, char *argv[]) {

    if (argc >= 4) {
        commandLineUser(*argv);
    } else if (argc == 2) {
        testExample();
    } else {
        consoleUser();
    }

}

void testExample() {

    AVLMap<Pixel, Image> sourceImageDictionary = AVLMap<Pixel, Image>();

    int imageSquareSizes = 25;

    sourceImageDictionary.loadDirectory("./sampleImages/smallLibrary", imageSquareSizes);

    // using Image in constructor
    Image squirrel("./sampleImages/srcImages/Squirrel.jpg");

    squirrel.readImage();

    Photomosaic squirrelMosaic(squirrel, sourceImageDictionary,
                               "./sampleImages/mosaics/squirrelMosaicSmallLib.png", imageSquareSizes);

    squirrelMosaic.createMosaic();

    squirrelMosaic.writeImage(Image::FileTypes::PNG);

    // using all file paths in constructor
    Photomosaic landscapeMosaic("./sampleImages/srcImages/landscape.jpg", sourceImageDictionary,
                                "./sampleImages/mosaics/landscapeMosaicSmallLib.png", imageSquareSizes);

    landscapeMosaic.createMosaic();

    landscapeMosaic.writeImage(Image::FileTypes::PNG);


    // using Image in constructor
    Image lighthouse("./sampleImages/srcImages/lighthouse4kimage.jpg");

    lighthouse.readImage();

    Photomosaic lighthouseMosaic(lighthouse, sourceImageDictionary,
                                 "./sampleImages/mosaics/lighthouseMosaicSmallLib.png", imageSquareSizes);

    lighthouseMosaic.createMosaic();

    lighthouseMosaic.writeImage(Image::FileTypes::PNG);
}

void commandLineUser(const char* argv) {
    string inputImagePath {argv[1]};
    string imageDirPath {argv[2]};
    string outputImagePath {argv[3]};

    const int imageSquareSizes = 25;

    AVLMap<Pixel, Image> sourceImageDictionary = AVLMap<Pixel, Image>();

    sourceImageDictionary.loadDirectory(imageDirPath, imageSquareSizes);

    Photomosaic commandLine(inputImagePath, sourceImageDictionary, outputImagePath, imageSquareSizes);
    commandLine.createMosaic();
    commandLine.writeImage(Image::FileTypes::PNG);
}

void consoleUser() {
    string inputImagePath{};
    string imageDirPath{};
    string outputImagePath{};
    string insertionSize;

    cout << "This program will automatically create a photomosaic "
            "once you've entered all the required values.\n";

    cout << "The program may through an error in the case it can't validate some input.\n" << endl;

    cout << "Please enter the input file path:\n" << endl;
    cin >> inputImagePath;
    cout << "Please enter the directory for the source image library:\n" << endl;
    cin >> imageDirPath;
    cout << "Please enter the output file path:\n" << endl;
    cin >> outputImagePath;
    cout << "Please enter the size you wish the insertions to be:" << endl;
    cin >> insertionSize;

    const int imageSquareSizes = std::stoi(insertionSize);

    AVLMap<Pixel, Image> sourceImageDictionary = AVLMap<Pixel, Image>();

    sourceImageDictionary.loadDirectory(imageDirPath, imageSquareSizes);

    Photomosaic photomosaic(inputImagePath, sourceImageDictionary, outputImagePath, imageSquareSizes);

    photomosaic.createMosaic();

    photomosaic.writeImage(Image::FileTypes::PNG);

    cout << "Success, please check your photomosaic!" << endl;
}
