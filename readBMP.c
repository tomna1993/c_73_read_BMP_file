#include <stdio.h>
#include <stdlib.h>

typedef struct BMP
{
    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    __int8 Id1;
    __int8 Id2;

    // Size of BMP file in bytes
    __int32 Size;

    // 4 bytes reserved
    __int32 Reserved;

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


__int8 func_read_bmp_header(BMP buffer, const char *file_name);
__int8 func_read_file(void *address, FILE *file_pointer);

int main(int argc, char **argv)
{
    if (argc <= 1 || argc > 2)
    {
        printf ("Usage: ./readBMP smiley.bmp\n");
        return EXIT_FAILURE;
    }
    
    BMP picture;

    func_read_bmp_header(picture, argv[1]);

    return EXIT_SUCCESS;
}

__int8 func_read_bmp_header(BMP buffer, const char *file_name)
{
    FILE *fp = fopen(file_name, "rb");

    if (fp == NULL)
    {
        printf ("Cannot open the file!\n");
        return EXIT_FAILURE;
    }

    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    int is_fail = func_read_file(&buffer.Id1, fp);
    if (is_fail) return EXIT_FAILURE;

    is_fail = func_read_file(&buffer.Id2, fp);
    if (is_fail) return EXIT_FAILURE;

    // Size of BMP file in bytes
    is_fail = func_read_file(&buffer.Size, fp);
    if (is_fail) return EXIT_FAILURE;

    // 4 bytes reserved
    if (fseek(fp, sizeof(__int32), SEEK_CUR) != 0)
    {
        printf ("fseek() failed\n");
        return EXIT_FAILURE;
    }

    // Starting address of the byte where the bitmap image data (pixel array) can be found
    is_fail = func_read_file(&buffer.Data_array_address, fp);
    if (is_fail) return EXIT_FAILURE;

    // DIB (bitmap information header)
    // Size of this header, in bytes (40)
    is_fail = func_read_file(&buffer.Header_size, fp);
    if (is_fail) return EXIT_FAILURE;

    // Bitmap width in pixels (signed integer)
    is_fail = func_read_file(&buffer.Width, fp);
    if (is_fail) return EXIT_FAILURE;

    // Bitmap height in pixels (signed integer)
    is_fail = func_read_file(&buffer.Height, fp);
    if (is_fail) return EXIT_FAILURE;

    // Number of color planes (must be 1)
    is_fail = func_read_file(&buffer.Color_planes, fp);
    if (is_fail) return EXIT_FAILURE;

    // Number of bits per pixel
    is_fail = func_read_file(&buffer.Bits_per_pixel, fp);
    if (is_fail) return EXIT_FAILURE;

    // Compression method
    is_fail = func_read_file(&buffer.Comp_method, fp);
    if (is_fail) return EXIT_FAILURE;

    // Size of the raw bitmap data
    is_fail = func_read_file(&buffer.Bitmap_size, fp);
    if (is_fail) return EXIT_FAILURE;

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    is_fail = func_read_file(&buffer.H_res, fp);
    if (is_fail) return EXIT_FAILURE;

    // Vertical resolution of the image. (pixel per metre, signed integer)
    is_fail = func_read_file(&buffer.V_res, fp);
    if (is_fail) return EXIT_FAILURE;

    // Number of colors in the color palette
    is_fail = func_read_file(&buffer.Num_colors_palette, fp);
    if (is_fail) return EXIT_FAILURE;

    // Number of important colors
    is_fail = func_read_file(&buffer.Num_imp_col, fp);
    if (is_fail) return EXIT_FAILURE;
    
    fclose(fp);

    return EXIT_SUCCESS;
}

__int8 func_read_file(void *address, FILE *file_pointer)
{
    fread(address, sizeof(*address), 1, file_pointer);

    if (feof(file_pointer))
    {
        printf ("Error reading the file: unexpected end of file!\n");
        return EXIT_FAILURE;
    }
    else if (ferror(file_pointer))
    {
        printf ("Error reading the file!\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
