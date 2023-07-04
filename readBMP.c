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


BMP func_read_bmp_header(const char *file_name);
void func_print_BMP_info(BMP picture_struct);

int main(int argc, char **argv)
{
    if (argc <= 1 || argc > 2)
    {
        printf ("Usage: ./readBMP smiley.bmp\n");
        return EXIT_FAILURE;
    }
    
    BMP picture;

    picture = func_read_bmp_header(argv[1]);

    func_print_BMP_info(picture);

    return EXIT_SUCCESS;
}

BMP func_read_bmp_header(const char *file_name)
{
    FILE *fp = fopen(file_name, "rb");

    BMP header;

    if (fp == NULL)
    {
        printf ("Cannot open the file!\n");
        return header;
    }

    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    fread(&header.Id1, sizeof(header.Id1), 1, fp);
    fread(&header.Id2, sizeof(header.Id2), 1, fp);

    // Size of BMP file in bytes
    fread(&header.Size, sizeof(header.Size), 1, fp);

    // 4 bytes reserved
    fseek(fp, sizeof(__int32), SEEK_CUR);

    // Starting address of the byte where the bitmap image data (pixel array) can be found
    fread(&header.Data_array_address, sizeof(header.Data_array_address), 1, fp);

    // DIB (bitmap information header)
    // Size of this header, in bytes (40)
    fread(&header.Header_size, sizeof(header.Header_size), 1, fp);

    // Bitmap width in pixels (signed integer)
    fread(&header.Width, sizeof(header.Width), 1, fp);

    // Bitmap height in pixels (signed integer)
    fread(&header.Height, sizeof(header.Height), 1, fp);

    // Number of color planes (must be 1)
    fread(&header.Color_planes, sizeof(header.Color_planes), 1, fp);

    // Number of bits per pixel
    fread(&header.Bits_per_pixel, sizeof(header.Bits_per_pixel), 1, fp);

    // Compression method
    fread(&header.Comp_method, sizeof(header.Comp_method), 1, fp);

    // Size of the raw bitmap data
    fread(&header.Bitmap_size, sizeof(header.Bitmap_size), 1, fp);

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    fread(&header.H_res, sizeof(header.H_res), 1, fp);

    // Vertical resolution of the image. (pixel per metre, signed integer)
    fread(&header.V_res, sizeof(header.V_res), 1, fp);

    // Number of colors in the color palette
    fread(&header.Num_colors_palette, sizeof(header.Num_colors_palette), 1, fp);

    // Number of important colors
    fread(&header.Num_imp_col, sizeof(header.Num_imp_col), 1, fp);
    
    fclose(fp);

    return header;
}

void func_print_BMP_info(BMP picture_struct)
{
    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    printf("BMP identification: %c%c\n", picture_struct.Id1, picture_struct.Id2);

    // Size of BMP file in bytes
    printf("Size: %i\n", picture_struct.Size);

    // Starting address of the byte where the bitmap image data (pixel array) can be found
    printf("Bitmap image data starting address: %x\n", picture_struct.Data_array_address);

    // DIB (bitmap information header)
    // Size of this header, in bytes (40)
    printf("Size of DIB header: %i\n", picture_struct.Header_size);

    // Bitmap width in pixels (signed integer)
    printf("BMP width: %i\n", picture_struct.Width);

    // Bitmap height in pixels (signed integer)
    printf("BMP height: %i\n", picture_struct.Height);

    // Number of color planes (must be 1)
    printf("Number of color planes: %i\n", picture_struct.Color_planes);

    // Number of bits per pixel
    printf("Number of bits per pixel: %i\n", picture_struct.Bits_per_pixel);

    // Compression method
    printf("Compression method: %i\n", picture_struct.Comp_method);

    // Size of the raw bitmap data
    printf("Size of the raw bitmap data: %i\n", picture_struct.Bitmap_size);

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    printf("Horizontal resolution: %i\n", picture_struct.H_res);

    // Vertical resolution of the image. (pixel per metre, signed integer)
    printf("Vertical resolution: %i\n", picture_struct.V_res);

    // Number of colors in the color palette
    printf("Number of colors in the color palette: %i\n", picture_struct.Num_colors_palette);

    // Number of important colors
    printf("Number of important colors: %i\n", picture_struct.Num_imp_col);

    return EXIT_SUCCESS;
}
