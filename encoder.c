#include <stdio.h>
#include <png.h>
#include <assert.h>
#include "encoder.h"

extern png_structp png_ptr = NULL;
extern png_infop info_ptr = NULL;

int libpng_init(char* output, int width, int height)
{
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if(!png_ptr)
    {
        return 1;
    }

    info_ptr = png_create_info_struct(png_ptr);

    if (!info_ptr)
    {
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        return 1;
    }

    FILE* fp = fopen(output, "wb");
    if (!fp) {
        png_destroy_write_struct(&png_ptr, &info_ptr);
        return 1;
    }
    png_init_io(png_ptr, fp);

    png_set_IHDR(
        png_ptr, 
        info_ptr, 
        width, 
        height, 
        8, 
        PNG_COLOR_TYPE_GRAY,     
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    return 0;
}

int png_get_input_bytes(char* filename, char* buffer, int buffersize)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        return 1;
    }

    int bytesRead = fread(buffer, 1, buffersize, fp);

    if(bytesRead <= 0)
    {
        return 1;
    }

    printf("%d byte(s) were read from %s.\n", bytesRead, filename);

    return 0;
}