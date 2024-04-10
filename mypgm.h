/* pgm file IO headerfile ------ mypgm.h */

#include <cstdio>
#include <stdio.h>
#include <stdlib.h>

/* Constant declaration */

//#define MAX_IMAGESIZE  1024
#define MAX_IMAGEWIDTH  1024
#define MAX_IMAGEHEIGHT  1024
#define MAX_BRIGHTNESS  255     /* Maximum gray level */
#define GRAYLEVEL       256     /* No. of gray levels */
#define MAX_FILENAME    256     /* Filename length limit */
#define MAX_BUFFERSIZE  256


/* Global constant declaration */
float image1[MAX_IMAGEWIDTH][MAX_IMAGEHEIGHT], image2[MAX_IMAGEWIDTH][MAX_IMAGEHEIGHT], image3[MAX_IMAGEWIDTH][MAX_IMAGEHEIGHT];     /* Image storage arrays */
int x_size1, y_size1, x_size2, y_size2, x_size3, y_size3;      /* width and height of image1 & width and height of image2 */


/* Prototype declaration of functions */
void load_image_data();         /* image input */
void save_image_data();         /* image output */
void load_image_file(char*);    /* image input */
void save_image_file(char*);    /* image output */


/* Main body of functions */
void load_image_data()

/* Input of header & body information of pgm file */
/* for image1[ ][ ]，x_size1，y_size1 */
{
    char file_name[MAX_FILENAME];
    char buffer[MAX_BUFFERSIZE];
    char bufferOutput[MAX_BUFFERSIZE];
    FILE* fp;       /* File pointer */
    int max_gray;   /* Maximum gray level */
    int x, y;       /* Loop variable */

    /* Input file open */
    printf("\n------------------------------------------------ ------------------------------");
    printf("\nBEGINNING: Loading image1 (INPUT IMAGES)");

    printf("\n !!!WARNING!!! Only 1024x1024 PGM files are acceptable\n\n");
    printf(" Input image file name? (*.pgm) : ");

    scanf("%s", file_name);

    fp = fopen(file_name, "rb");
    if (NULL == fp) {
        printf(" File does not exist...!\n\n");
        exit(1);
    }

    /* Check of file-type ---P5 */
    fgets(buffer, MAX_BUFFERSIZE, fp);
    //printf("string is: %s\n", buffer);

    if (buffer[0] != 'P' || buffer[1] != '5') {
        printf(" Wrong format...\n\n");
        exit(1);
    }

    /* input of x_size1, y_size1 */
    x_size1 = 0;
    y_size1 = 0;
    while (x_size1 == 0 || y_size1 == 0) {
        fgets(buffer, MAX_BUFFERSIZE, fp);
        if (buffer[0] != '#') {
            sscanf(buffer, "%d %d", &x_size1, &y_size1);
        }
    }
    /* input of max_gray */
    max_gray = 0;
    while (max_gray == 0) {
        fgets(buffer, MAX_BUFFERSIZE, fp);
        if (buffer[0] != '#') {
            sscanf(buffer, "%d", &max_gray);
        }
    }
    /* Display of parameters */
    printf("\n");
    printf(" Image width = %d OK\n", x_size1);
    printf(" Image height = %d OK\n", y_size1);
    printf(" Gray level = %d OK\n\n", max_gray);

    if (x_size1 > MAX_IMAGEWIDTH || y_size1 > MAX_IMAGEHEIGHT) {
        printf(" Image size exceeds %d x %d\n\n",
            MAX_IMAGEWIDTH, MAX_IMAGEHEIGHT);
        printf(" Please use smaller images!\n\n");
        exit(1);
    }
    if (max_gray != MAX_BRIGHTNESS) {
        printf(" Invalid value of maximum gray level!\n\n");
        exit(1);
    }

    /* Input of image data*/
    for (y = 0; y < y_size1; y++) {
        for (x = 0; x < x_size1; x++) {
            image1[y][x] = (unsigned char)fgetc(fp);
        }
    }
    printf("\nCOMPLETE: Loading image1 (Input images) - SUCCESS!");
    printf("\n------------------------------------------------ -------------------------------\n\n\n");
    fclose(fp);
}

void save_image_data_img2()
/* Output of image2[ ][ ], x_size2, y_size2 in pgm format*/

{
    char file_name[MAX_FILENAME];
    FILE* fp; /* File pointer */
    int x, y; /* Loop variable */

    /* Output file open */
    printf("------------------------------------------------ -----------------------------");
    printf("\nBEGINNING: Saving image2 (Image_GPU)\n");

    printf("\n Name of the output image file? (*.pgm) : ");
    scanf("%s", file_name);
    fp = fopen(file_name, "wb");
    /* output of pgm file header information */
    fputs("P5\n", fp);
    fputs("# Created by Image Processing\n", fp);
    fprintf(fp, "%d %d\n", x_size2, y_size2);
    fprintf(fp, "%d\n", MAX_BRIGHTNESS);
    /* Output of image data */
    for (y = 0; y < y_size2; y++) {
        for (x = 0; x < x_size2; x++) {
            fputc(image2[y][x], fp);
        }
    }
    printf("\nCOMPLETE: Saving image2 (Image_GPU) - SUCCESS!\n");
    printf("------------------------------------------------ ------------------------------\n\n\n");
    fclose(fp);
}
void save_image_data_img3()
/* Output of image2[ ][ ], x_size2, y_size2 in pgm format*/

{
    char file_name[MAX_FILENAME];
    FILE* fp; /* File pointer */
    int x, y; /* Loop variable */

    /* Output file open */
    printf("------------------------------------------------ -----------------------------");
    printf("\nBEGIN: Saving image3 (Image_CPU)\n");

    printf("\n Name of the output image file? (*.pgm) : ");
    scanf("%s", file_name);
    fp = fopen(file_name, "wb");
    /* output of pgm file header information */
    fputs("P5\n", fp);
    fputs("# Created by Image Processing\n", fp);
    fprintf(fp, "%d %d\n", x_size3, y_size3);
    fprintf(fp, "%d\n", MAX_BRIGHTNESS);
    /* Output of image data */
    for (y = 0; y < y_size3; y++) {
        for (x = 0; x < x_size3; x++) {
            fputc(image3[y][x], fp);
        }
    }
    printf("\nCOMPLETE: Saving image3 (Image_CPU) - SUCCESS!\n");
    printf("------------------------------------------------ ------------------------------\n\n\n");
    fclose(fp);
}

void load_image_file(char* filename)
/* Input of header & body information of pgm file */
/* for image1[ ][ ]，x_size1，y_size1 */
{
    char buffer[MAX_BUFFERSIZE];
    FILE* fp; /* File pointer */
    int max_gray; /* Maximum gray level */
    int x, y; /* Loop variable */

    /* Input file open */
    fp = fopen(filename, "rb");
    if (NULL == fp) {
        printf(" The file doesn't exist!\n\n");
        exit(1);
    }
    /* Check of file-type ---P5 */
    fgets(buffer, MAX_BUFFERSIZE, fp);
    if (buffer[0] != 'P' || buffer[1] != '5') {
        printf(" Mistaken file format, not P5!\n\n");
        exit(1);
    }
    /* input of x_size1, y_size1 */
    x_size1 = 0;
    y_size1 = 0;
    while (x_size1 == 0 || y_size1 == 0) {
        fgets(buffer, MAX_BUFFERSIZE, fp);
        if (buffer[0] != '#') {
            sscanf(buffer, "%d %d", &x_size1, &y_size1);
        }
    }
    /* input of max_gray */
    max_gray = 0;
    while (max_gray == 0) {
        fgets(buffer, MAX_BUFFERSIZE, fp);
        if (buffer[0] != '#') {
            sscanf(buffer, "%d", &max_gray);
        }
    }
    if (x_size1 > MAX_IMAGEWIDTH || y_size1 > MAX_IMAGEHEIGHT) {
        printf(" Image size exceeds %d x %d\n\n",
            MAX_IMAGEWIDTH, MAX_IMAGEHEIGHT);
        printf(" Please use smaller images!\n\n");
        exit(1);
    }
    if (max_gray != MAX_BRIGHTNESS) {
        printf(" Invalid value of maximum gray level!\n\n");
        exit(1);
    }
    /* Input of image data*/
    for (y = 0; y < y_size1; y++) {
        for (x = 0; x < x_size1; x++) {
            image1[y][x] = (float)fgetc(fp);
        }
    }
    fclose(fp);
}

void save_image_file(char* filename)
/* Output of image2[ ][ ], x_size2, y_size2 */
/* into pgm file with header & body information */
{
    FILE* fp; /* File pointer */
    int x, y; /* Loop variable */

    fp = fopen(filename, "wb");
    /* output of pgm file header information */
    fputs("P5\n", fp);
    fputs("# Created by Image Processing\n", fp);
    fprintf(fp, "%d %d\n", x_size2, y_size2);
    fprintf(fp, "%d\n", MAX_BRIGHTNESS);
    /* Output of image data */
    for (y = 0; y < y_size2; y++) {
        for (x = 0; x < x_size2; x++) {
            fputc(image2[y][x], fp);
        }
    }
    fclose(fp);
}