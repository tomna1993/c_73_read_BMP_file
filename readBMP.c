#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define BYTE_IN_PIXEL 3

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

    // Each row length of the BMP mut be a multiple of four bytes. If there is a missing
    // byte, f.e. the row has 1 pixel which contains 3 bytes (blue, green, red) then 
    // there will be a missing 4th byte. That has to be inserted at the end of the row.
    // Calculate the number of bytes to add after each row
    __int8 Num_bytes_padding;

    // If picture.Height is negative then:
    // - read the file from top-left pixel; read rows left-rigth and top-down
    // If picture.Height is positive then:
    // - read the file from bottom-left pixel; read rows left-rigth and bottom-up
    bool Is_top_down_method;

    __int32 Pixel_count;

    __int32 Byte_count;
}
BMP;

typedef struct PIXEL
{
    __int8 Blue;
    __int8 Green;
    __int8 Red;
}
PIXEL;

BMP func_read_bmp_header(const char *file_name);
void func_print_BMP_info(BMP picture_struct);
__int8 func_read_data_array(PIXEL *pixel, const char *file_name, BMP *picture);
void func_print_data_array(PIXEL *pixel, BMP *picture);
__int8 func_create_bmp(const char *file_name, BMP *picture, PIXEL *pixel);
void func_change_color(BMP *picture, PIXEL *pixel, __int8 blue, __int8 green, __int8 red);


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



    // Each row length of the BMP mut be a multiple of four bytes. If there is a missing
    // byte, f.e. the row has 1 pixel which contains 3 bytes (blue, green, red) then 
    // there will be a missing 4th byte. That has to be inserted at the end of the row.
    // Calculate the number of bytes to add after each row 
    picture.Num_bytes_padding = (picture.Width * BYTE_IN_PIXEL) % 4;

    if (picture.Num_bytes_padding != 0)
    {
        picture.Num_bytes_padding = 4 - picture.Num_bytes_padding;
    }
    

    // If picture.Height is negative then:
    // - read the file from top-left pixel; read rows left-rigth and top-down
    if (picture.Height < 0)
    {
        picture.Pixel_count = picture.Width * (picture.Height * -1);  
        picture.Is_top_down_method = true; 
    }
    // If picture.Height is positive then:
    // - read the file from bottom-left pixel; read rows left-rigth and bottom-up
    else
    {
        picture.Pixel_count = picture.Width * picture.Height;
        picture.Is_top_down_method = false;
    }

    // Declare them to be constants just to be shure I don't modify them later
    picture.Byte_count = picture.Pixel_count * BYTE_IN_PIXEL;

    // make it work with memory allocation
    PIXEL *pixel = calloc(picture.Byte_count, sizeof(__int8));

    __int8 is_error = func_read_data_array(pixel, argv[1], &picture);

    if (is_error == EXIT_FAILURE)
    {
        printf ("Reading the BMP data failed!\n");
        return EXIT_FAILURE;
    }

    func_print_data_array(pixel, &picture);

    func_change_color(&picture, pixel, 27, 154, 255);


    func_create_bmp("output.bmp", &picture, pixel);

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
}

__int8 func_read_data_array(PIXEL *pixel, const char *file_name, BMP *picture)
{
    FILE *fp = fopen(file_name, "rb");

    if (fp == NULL)
    {
        printf ("Cannot open the file!\n");
        return EXIT_FAILURE;
    }

    fseek(fp, picture->Data_array_address, SEEK_SET);

    for (int i = 0; i < picture->Pixel_count; i++)
    {
        // After each row move the file pointer with number of padding bytes
        if (i != 0 && (i % picture->Width) == 0)
        {
            fseek(fp, picture->Num_bytes_padding, SEEK_CUR);
        }

        fread(&((pixel + i)->Blue), sizeof(__int8), 1, fp);
        fread(&((pixel + i)->Green), sizeof(__int8), 1, fp);
        fread(&((pixel + i)->Red), sizeof(__int8), 1, fp);
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

void func_print_data_array(PIXEL *pixel, BMP *picture)
{
    if (picture->Is_top_down_method)
    {
        for (int i = 0; i < picture->Pixel_count; i++)
        {
            if (i % picture->Width == 0)
            {
                printf ("\n");
            }

            if((pixel + i)->Blue == 0 && (pixel + i)->Green == 0 && (pixel + i)->Red == 0)
            {
                printf("1 ");
            }
            else
            {
                printf ("0 ");
            }
        }
    }
    else
    {
        for(__int32 row = picture->Height; row > 0; row--)
        {
            // Start printing from top-left pixel; print rows top-down 
            __int32 row_start = (row * picture->Width) - picture->Width;
            __int32 row_end = (row * picture->Width);

            for (int i = row_start; i < row_end; i++)
            {
                if(( pixel + i)->Blue     == 0 && 
                    (pixel + i)->Green    == 0 && 
                    (pixel + i)->Red      == 0)
                {
                    printf("1 ");
                }
                else
                {
                    printf ("0 ");
                }
            }

            printf ("\n");
        } 
    }
}

__int8 func_create_bmp(const char *file_name, BMP *picture, PIXEL *pixel)
{
    FILE *fp = fopen(file_name, "wb");

    if (fp == NULL)
    {
        printf ("Cannot open/create the file!\n");
        return EXIT_FAILURE;
    }

    // 2 bytes to identify the BMP and DIB file
    // For BMP it should be BM (0x42 0x4D in hexadecimal)
    fwrite(&(picture->Id1), sizeof(picture->Id1), 1, fp);
    fwrite(&(picture->Id2), sizeof(picture->Id2), 1, fp);

    // Size of BMP file in bytes
    fwrite(&(picture->Size), sizeof(picture->Size), 1, fp);

    // 4 bytes reserved
    fputc(0, fp);
    fputc(0, fp);
    fputc(0, fp);
    fputc(0, fp);

    // Starting address of the byte where the bitmap image data (pixel array) can be found
    fwrite(&(picture->Data_array_address), sizeof(picture->Data_array_address), 1, fp);

    // DIB (bitmap information header)
    // Size of this header, in bytes (40)
    fwrite(&(picture->Header_size), sizeof(picture->Header_size), 1, fp);

    // Bitmap width in pixels (signed integer)
    fwrite(&(picture->Width), sizeof(picture->Width), 1, fp);

    // Bitmap height in pixels (signed integer)
    fwrite(&(picture->Height), sizeof(picture->Height), 1, fp);

    // Number of color planes (must be 1)
    fwrite(&(picture->Color_planes), sizeof(picture->Color_planes), 1, fp);

    // Number of bits per pixel
    fwrite(&(picture->Bits_per_pixel), sizeof(picture->Bits_per_pixel), 1, fp);

    // Compression method
    fwrite(&(picture->Comp_method), sizeof(picture->Comp_method), 1, fp);

    // Size of the raw bitmap data
    fwrite(&(picture->Bitmap_size), sizeof(picture->Bitmap_size), 1, fp);

    // Horizontal resolution of the image. (pixel per metre, signed integer)
    fwrite(&(picture->H_res), sizeof(picture->H_res), 1, fp);

    // Vertical resolution of the image. (pixel per metre, signed integer)
    fwrite(&(picture->V_res), sizeof(picture->V_res), 1, fp);

    // Number of colors in the color palette
    fwrite(&(picture->Num_colors_palette), sizeof(picture->Num_colors_palette), 1, fp);

    // Number of important colors
    fwrite(&(picture->Num_imp_col), sizeof(picture->Num_imp_col), 1, fp);
    
    for (int i = 0; i < picture->Pixel_count; i++)
    {
        // After each row move the file pointer with number of padding bytes
        if (i != 0 && (i % picture->Width) == 0)
        {
            for (int i = 0; i < picture->Num_bytes_padding; i++)
            {
                fputc(0, fp);
            }
        }

        fwrite(&((pixel + i)->Blue), sizeof(__int8), 1, fp);
        fwrite(&((pixel + i)->Green), sizeof(__int8), 1, fp);
        fwrite(&((pixel + i)->Red), sizeof(__int8), 1, fp);
    }

    if (picture->Num_bytes_padding > 0)
    {
        for (int i = 0; i < picture->Num_bytes_padding; i++)
        {
            fputc(0, fp);
        }
    }

    fclose(fp);

    return EXIT_SUCCESS;
}

void func_change_color(BMP *picture, PIXEL *pixel, __int8 blue, __int8 green, __int8 red)
{
    for (int i = 0; i < picture->Pixel_count; i++)
    {
        if((pixel + i)->Blue == 0 && (pixel + i)->Green == 0 && (pixel + i)->Red == 0)
        {
            (pixel + i)->Blue   = blue;
            (pixel + i)->Green  = green;
            (pixel + i)->Red    = red;
        }
    }
}