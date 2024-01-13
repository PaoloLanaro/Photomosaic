# [Photomosaic C++](https://github.com/PaoloLanaro/Photomosaic)

by [Paolo Lanaro](https://github.com/PaoloLanaro)

Creates a simple photomosaic for a source image out of a databank of other images.

## Description

This project aims to create [photomosaics](https://en.wikipedia.org/wiki/Photographic_mosaic) out of images provided. 
The user can both input the databank images, which will refer to the images composing the photomosaic, and a source 
image, which is the image the user wants to actually create a photomosaic for. There are examples of the program running
with a databank of ~9000 images under ./sampleImages/mosaics/fullDatabankMosaics. The project uses a AVLMap 
implementation I created in C++, split among the .cpp and .h files in the library, which allows for a quicker storing
of the databank images. The time complexity of the running of the program is therefore cut down as I use a fairly 
efficient data structure to possibly store thousands of images. 

## Getting Started

### Dependencies

* This program was built on the [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux) using CMake version 
3.22.1

### Installing

* The main thing you want to take note of for running this program is your databank's path. I personally recommend using
a folder with lots of images, such as this [Kaggle set](https://www.kaggle.com/datasets/crawford/cat-dataset). The 
databank path will be wherever you've saved these images you would like to use, and just has to be the parent directory
to the images.
* You should also save the path to the image you would like to create a phtomosaic of, and if the tiles are too big or 
small, you can either modify these in the driver.cpp file, or run the program with no command line args and use the 
console.

### Executing program

Pass in 3 command line arguments when executing the program. Alternatively run it with no command line arguments and 
use the console instructions.

* 3 command line arguments should be the following:
```
sourceImagePath databankImagesPath outputImagePath
```
1) The path to the image you would like to create a photomosaic of.
2) The path to the databank of images you would like to use to create the photomosaic.
3) The path you would like to output the image to.

## Help

The program may take a long time to run, and this is just a consequence of not being able to make some tweaks in time
for the assignment deadline. If you find the program takes way too long to run (5+ minutes), please contact me! 

If you're having issues with opening the image, make sure the output file includes `.png` after the image's name.
```
/outputImagePath/outputImageName.png
```

There are examples of running the program with a small library and general running code in the testExample() function
in the driver.cpp file.

For any other issues, please contact me at my email.

## Acknowledgments

This program was made for a "Programming in C++" class assignment at Northeastern University. \
Dr. [Mark Fontenot](https://github.com/markfontenot) was a big help in questions about AVLMaps and general debugging. \
I used the [USFCA AVLTree visualizer](https://www.cs.usfca.edu/~galles/visualization/AVLtree.html) to understand 
the AVLTree and AVLMap. 
