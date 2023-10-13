# Portable Pixmap (PPM) Image Editor

This is a code created for the Assignment 01 of the Data Structures course, developed by Laura Boemo.

## How to Run

1. In this document, execute:

```Shell
$ gcc main.c -o main
```

2. To run the program:

```Shell
$ ./main
```

## Available Functional Requirements

- [x] Read images in PPM/P3 format
- [x] Write images in PPM/P3 format
- [x] Color Filter Application 01 - Contrast Enhancement
- [x] Color Filter Application 02 - Red Negation
- [x] Pixel Modification Filter Application 01 - Horizontal Rotation
- [x] Pixel Modification Filter Application 02 - Vertical Rotation
- [ ] Cropping Operation
- [ ] Pasting Operation
- [x] Scaling Operation
- [ ] Subset Selection

## Available Data Structure Requirements

- [x] Storage and manipulation of images using dynamically allocated memory blocks and pointers
- [x] Whenever possible, the application of filters/operations does not use additional memory blocks

## Report

This C code implements a simple image editing program in the PPM (Portable Pixmap) format. Data structures play a crucial role in the operation of the program, allowing for the representation, manipulation, and efficient processing of images. Let's analyze how data structures are applied in this code:

1. Pixel Structures (pixel):

   The pixel structure is used to represent the individual colors of pixels in the image. It contains three integer fields for the Red (R), Green (G), and Blue (B) color components.

2. PPM Image Structure (ppmFile):

   The ppmFile structure is used to represent the PPM image as a whole. It contains the following fields:

   - row: Represents the number of rows in the image.
   - col: Represents the number of columns in the image.
   - colorDepth: Represents the maximum color depth (usually 0-255) of the PPM image.
   - body: A pointer to an array of pixels that stores the image data.

3. Image Reading (readImage):

   The readImage function uses the ppmFile structure to read an input PPM file. It dynamically allocates memory to store the image data and populates the structure with information read from the file.

4. Image Writing (writeImage):

   The writeImage function uses the ppmFile structure to write the image data to a new output PPM file. It uses the structure's fields to create the file header and writes the image pixels.

5. Image Editing Filters (extremeContrast, negateRed, horizontalFlip, verticalFlip, zoom):

   Each of these functions applies a specific filter to the image. They use the ppmFile structure to access the image pixels and perform the necessary operations. For example, extremeContrast adjusts the image's contrast, while horizontalFlip horizontally flips the image. These functions update the pixel values in the ppmFile structure.

6. Options Menu:

   The code allows the user to select which filters to apply to the image through an interactive menu. User choices are stored in a character array `choice`, which is used to determine which filters to execute.

Overall, the data structures (pixel and ppmFile) are used to represent and manipulate image data, while functions operate on this data to apply the desired filters. The code is a practical example of how data structures are essential for processing complex information, such as images, in an organized and efficient manner.
