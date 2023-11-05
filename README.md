[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/A0poywh0)
# CS3520 - PA03 - Photomosaics

Developer Name: Paolo Lanaro

## How to Run the Program

There are three ways to run the program. \
You can choose to run the program with 3 command line arguments, \
one command line argument, or with no command line arguments. <br>

When ran with three command line arguments, the first should be a path \
to the image you wish to create a photomosaic of. The second argument \
should be the path to the databank (library) of images you wish to use to \
construct the photomosaic. The last argument should be the path to the file you wish \
the completed image to be output to. This method of running the program will \
result in 80 pixel sized insertions, so it might not be fit for every image. \
This method of running the program will also make the output a PNG style image. <br>

When ran with one command line arg, the program will not allow for any user input \
and will instead use a predefined example (src image, image databank, and output image) \
to produce a photomosaic of a lighthouse made of cats! <br>

When ran with no command line arguments, the user will be able to control the program \
from the console. There will be prompts as for what to do, and this way you can create \
a photomosaic out of paths you provide in the console. <br>

## Notes to TAs

You must first create an `AVLMap<Pixel, Image>` map and call `.loadDirectory(std::string, int)` \
where the string is the path to the image databank, and the int is the size you wish to resize the \ 
source images to.

To run the program in the intended fashion, you must instantiate an object of type `Photomosaic`.
There are two constructors for `Photomosaic`. <br> <br>
One which takes in `(Image a, std::string b, std::string c, int d)` \
    a) one Image object, \
    b) one map for the image database, \
    c) one file path for the output image, \
    d) and the length and width you want the replaced images to be (aka imageSquareSizes). <br> <br>

And one which takes in `(std::string a, std::string b, std::string c, int d)` \
    a) one file path for the src image. \
    b) one map for the image database, \
    c) one file path for the output image, \
    d) and the length and width you want the replaced images to be (aka imageSquareSizes). <br> <br>

If you have used the constructor which takes in an `Image` object, you must first initialize \
said `Image` object by running `Image.readImage()`. You can then proceed to the next step.

You must than run the `.createMosaic()` function on the `Photomosaic` object, which aims to \
do the "meat" of the construction of the mosaic.  \
This includes partitioning off the source image, and getting imageSquareSizes^2 chunks of the \
image, getting the average pixel value from that chunk, and replacing the source image with the \
image within the databank (AVLMap) that most closely matches the Pixel retrieved from the chunk. 

To get the output of the image, you must call `Photomosaic.writeImage(filetype)`, \
where filetype is an enum class object of `Image::Filetypes`. \
This will write the image to the preferred extension.