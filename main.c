/*

~~~~~~~~~~~~~~~~~~~~~~ PPM IMAGE EDITOR ~~~~~~~~~~~~~~~~~~~~~~~~~
                         By: Laura Boemo
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MIN_DEPTH 0
#define MAX_DEPTH 255

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                         Typedefining
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

// Pixel structure
typedef struct {
    int R, G, B;
} pixel;

// PPM image structure
typedef struct {
    int row, col, colorDepth;
    pixel *body;
} ppmFile;

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                         Prototypes (Functions)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

// PPM Image Reading
ppmFile* readImage(char *imageFile);
// PPM Image Writing
void writeImage(char *imageName, ppmFile *imageFile);
// Color Filter
void extremeContrast(ppmFile* imageFile);
// Color Filter
void negateRed(ppmFile *imageFile);
// Pixel Modification Filter
void horizontalFlip(ppmFile *imageFile);
// Pixel Modification Filter
void verticalFlip(ppmFile *imageFile);
// Scaling Application Function
void zoom(ppmFile *imageFile);

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                         Initialization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
int main(void) {
    // Initializing input, output, and choices
    char inputFile[20], outputFile[20], choice[15];

    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("            Portable Pixmap (PPM) Image Editor            ");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    // Input of PPM file names
    printf("Enter the input image file name: ");
    scanf("%s", inputFile);

    printf("Enter the output image file name: ");
    scanf("%s", outputFile);

    // Assigning the read input file to the PPM file structure
    ppmFile *readFile = readImage(inputFile);

    puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("            Editing Options            ");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    // Declaration of PPM image editing options
    char options[5][100] = {
        "color for extreme contrast",
        "color to invert red colors",
        "pixel modification to horizontally flip the image",
        "pixel modification to vertically flip the image",
        "zoom in the image"
    };

    // Reading user choices for the provided options
    for (int i = 0; i < 5; ++i) {
        do {
            printf("Do you want to apply the %s filter? (y/n): ", options[i]);
            fflush(stdin);
            scanf(" %c", &choice[i]);
        } while ((choice[i] != 'y') && (choice[i] != 'Y') && (choice[i] != 'n') && (choice[i] != 'N');
    }

    // Calling and executing the selected options
    if (choice[0] == 'y' || choice[0] == 'Y')
        extremeContrast(readFile);

    if (choice[1] == 'y' || choice[1] == 'Y')
        negateRed(readFile);

    if (choice[2] == 'y' || choice[2] == 'Y')
        horizontalFlip(readFile);

    if (choice[3] == 'y' || choice[3] == 'Y')
        verticalFlip(readFile);

    if (choice[4] == 'y' || choice[4] == 'Y')
        zoom(readFile);

    puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("  Your new PPM image with filters is being processed...            ");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    // Writing the edited PPM image
    writeImage(outputFile, readFile);

    // Freeing dynamically allocated memory
    free(readFile->body);
    free(readFile);

    printf("%s is available!\n\n", outputFile);

    return EXIT_SUCCESS;
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                This function reads the input PPM file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
ppmFile* readImage(char *imageFile) {
    char imageFormat[4];
    FILE *fp; // File pointer for the input file

    // Allocating dynamic memory for the image to be read
    ppmFile *imagePtr = (ppmFile*)malloc(sizeof(ppmFile));
    if (!imagePtr) {
        printf("Error: Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    // Opening the PPM file in non-binary mode
    fp = fopen(imageFile, "r");

    if (!fp) { // If the file failed to open
        printf("Error: File opening failed!\n");
        exit(EXIT_FAILURE);
    }

    // Reading the PPM file and checking its format
    fgets(imageFormat, sizeof(imageFormat), fp);
    if (imageFormat[0] != 'P' || imageFormat[1] != '3') {
        printf("Error: Image format must be P3!\n");
        exit(EXIT_FAILURE);
    }

    // Reading the image size and its maximum color value
    fscanf(fp, "   %d %d   ", &imagePtr->col, &imagePtr->row);
    fscanf(fp, "  %d  ", &imagePtr->colorDepth);

    // Allocating dynamic memory for the pixel data
    imagePtr->body = (pixel*)malloc(imagePtr->row * imagePtr->col * sizeof(pixel));
    if (!imagePtr) {
        printf("Error: Memory allocation failed!\n");
        exit(EXIT_FAILURE);
    }

    // Reading the pixels that make up the image body
    for (int i = 0; i < imagePtr->row * imagePtr->col; ++i)
        fscanf(fp, "    %d %d %d    ", &(imagePtr->body[i].R), &(imagePtr->body[i].G), &(imagePtr->body[i].B));

    fclose(fp); // Closing the PPM file
    return imagePtr; // Returning the read file
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            This function writes the output PPM file
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void writeImage(char *imageName, ppmFile *imageFile) {
    FILE *fp; // File pointer for the output file

    // Creating the output file pointer
    fp = fopen(imageName, "w");
    if (!fp) { // If file creation fails
        printf("Error: Unable to create the file!\n");
        exit(EXIT_FAILURE);
    }

    // Writing the image header
    fprintf(fp, "P3"); // Magic number
    fprintf(fp, "   %d %d   ", imageFile->col, imageFile->row);  // Columns and Rows
    fprintf(fp, "  %d  ", imageFile->colorDepth); // Maximum color depth

    // Writing the image body
    for (int i = 0; i < (imageFile->row * imageFile->col); ++i)
        fprintf(fp, "    %d %d %d    ", imageFile->body[i].R, imageFile->body[i].G, imageFile->body[i].B);

    fclose(fp); // Closing the created image
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
            This function changes each color

 number to
            either the maximum possible or zero, by
            comparing it with half of the color depth.
            Generating extreme contrast.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void extremeContrast(ppmFile* imageFile) {
    // If the color number is greater than half of the color depth, it will be changed to 255, otherwise, 0
    for (int i = 0; i < imageFile->col * imageFile->row; i++) {
        if (imageFile->body[i].R > 128) // 128 = half of the color depth
            imageFile->body[i].R = MAX_DEPTH;
        else
            imageFile->body[i].R = MIN_DEPTH;

        if (imageFile->body[i].G > 128)
            imageFile->body[i].G = MAX_DEPTH;
        else
            imageFile->body[i].G = MIN_DEPTH;

        if (imageFile->body[i].B > 128)
            imageFile->body[i].B = MAX_DEPTH;
        else
            imageFile->body[i].B = MIN_DEPTH;
    }
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         This function negates the red number in each pixel
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void negateRed(ppmFile *imageFile) {
    // If red is low in value, it will become high, and vice versa
    for (int i = 0; i < imageFile->col * imageFile->row; i++)
        imageFile->body[i].R = abs(imageFile->body[i].R - MAX_DEPTH);
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         This function horizontally rotates the image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void horizontalFlip(ppmFile *imageFile) {
    // Shifting the center of the image
    int imageCenter = imageFile->col / 2;

    for (int i = 0; i < imageFile->row; ++i) {
        pixel *tmpArray = imageFile->body + i * imageFile->col; // Temporary array of Pixels
        for (int j = 0; j < imageCenter; ++j) {
            // Inverting Pixels
            pixel temp = tmpArray[j];
            tmpArray[j] = tmpArray[imageFile->col - 1 - j];
            tmpArray[imageFile->col - 1 - j] = temp;
        }
    }
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         This function vertically rotates the image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/
void verticalFlip(ppmFile *imageFile) {
    int line_size = imageFile->col * sizeof(pixel);
    int imageCenter = imageFile->row / 2;

    // Allocating a swap buffer
    pixel* buff = (pixel*)malloc(line_size);
    pixel* top;
    pixel* bottom;

    for (int y = 0; y < imageCenter; ++y) {
        // Swapping lines from the top and bottom
        top = imageFile->body  + (y * imageFile->col);
        bottom = imageFile->body  + ((imageFile->col - y - 1) * imageFile->col);

        memcpy(buff, top, line_size);
        memcpy(top, bottom, line_size);
        memcpy(bottom, buff, line_size);
    }

    free(buff); // Freeing the swap buffer
}

/*

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
         This function applies scaling (zoom) to the image
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/

void zoom(ppmFile *imageFile) {
    char inputZoom[20];
    int zoomValue = 1;
    FILE *fp; // File pointer for the output file

    puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("        Special Zoom Editions in PPM            ");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    printf("Enter the zoom image file name: ");
    scanf("%s", inputZoom);

    printf("Enter how much zoom you want: ");
    scanf("%d", &zoomValue);

    // Creating the output file pointer
    fp = fopen(inputZoom, "w");
    if (!fp) { // If file creation fails
        printf("Error: Unable to create the file!\n");
        exit(EXIT_FAILURE);
    }

    // Writing the image header
    fprintf(fp, "P3"); // Magic number
    fprintf(fp, "   %d %d   ", (imageFile->col * zoomValue), (imageFile->row * zoomValue));  // Columns and Rows with added zoom
    fprintf(fp, "  %d  ", imageFile->colorDepth); // Maximum color depth

    for (int i = 0; i < ((imageFile->row) * (imageFile->col)); i++) {
        for (int k = 0; k < zoomValue * zoomValue; k++) {
            fprintf(fp, "    %d %d %d    ",
                (imageFile->body[i].R > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].R,
                (imageFile->body[i].G > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].G,
                (imageFile->body[i].B > imageFile->colorDepth) ? imageFile->colorDepth : imageFile->body[i].B
            );
        }
    }

    puts("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    puts("  Your new PPM image with filters and zoom is being processed...            ");
    puts("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");

    fclose(fp); // Closing the created image

    printf("%s is available!", inputZoom);
}
