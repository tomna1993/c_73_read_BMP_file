#include <stdio.h>
#include <stdlib.h>

typedef struct BMP
{
    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    __int16 Id;

    // Size of BMP file in bytes
    __int32 Size;

    // 4 bytes reserved

    // Starting address of the byte where the bitmap image data (pixel array) can be found
    __int32 Data_array_address;

    // DIB (bitmap information header)
    // Size of this header, in bytes (40)
    __int32 Header_size;

    // Bitmap width in pixels (signed integer)
    __int32 Width;
    // Bitmap height in pixels (signed integer)
    __int32 Height;

    // Number of color planes (must be 1)
    __int16 Color_planes;

    // Number of bits per pixel
    __int16 Bits_per_pixel;

    // Compression method
    __int32 Comp_method;

    // Size of the raw bitmap data
    __int32 Bitmap_size;

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    __int32 H_res;

    // Vertical resolution of the image. (pixel per metre, signed integer)
    __int32 V_res;

    // Number of colors in the color palette
    __int32 Num_colors_palette;

    // Number of important colors
    __int32 Num_imp_col;
}
BMP;

int main(int argc, char **argv)
{
    if (argc <= 1 || argc > 2)
    {
        printf ("Usage: ./readBMP smiley.bmp\n");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "rb");

    if (fp == NULL)
    {
        printf ("Cannot open the file!\n");
        return EXIT_FAILURE;
    }

    fclose(fp);

    return EXIT_SUCCESS;
}