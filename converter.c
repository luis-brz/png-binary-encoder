#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include "converter.h"

#define BUFFERSIZE 1024 
#define PNG_WIDTH 10
#define PNG_HEIGHT 1

png_structp png_ptr;
png_infop info_ptr;

// gcc -o converter converter.c -Llibpng -Ilibpng -lpng -lz
// converter generated.png text.txt

int libpng_init(char* output)
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
        PNG_WIDTH, 
        PNG_HEIGHT, 
        8, 
        PNG_COLOR_TYPE_GRAY,     
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    return 0;
}

int get_input_bytes(char* filename, char* buffer)
{
    FILE* fp = fopen(filename, "rb");
    if(fp == NULL)
    {
        return 1;
    }

    fread(buffer, 1, BUFFERSIZE, fp);

    return 0;
}

int main(int argc, char* argv[])
{
    if(libpng_init(argv[1]) != 0)
    {
        fprintf(stderr, "There was an error with libpng!\n");
        return -1;
    }

    int bufferpos = 0;
    int buffershift = 7;
    char* buffer = malloc(BUFFERSIZE);
    memset(buffer, 0, BUFFERSIZE); 

    if(get_input_bytes(argv[2], buffer) != 0)
    {
        fprintf(stderr, "There was an error reading %s!\n", argv[2]);
        return -1;
    }

    for(int row = 0; row < PNG_HEIGHT; row++)
    {
        png_bytep row_pointer = (png_bytep) malloc(PNG_WIDTH * sizeof(png_byte));
        memset(row_pointer, 0, PNG_WIDTH * sizeof(png_byte));

        for(int col = 0; col < PNG_WIDTH; col++)
        {
            if(buffershift < 0)
            {
                bufferpos++;
                buffershift = 7;
            }

            if(((buffer[bufferpos] >> buffershift) & 1) == 1)
            {
                row_pointer[col] = 255;
            }
            else
            {
                row_pointer[col] = 0;
            }

            buffershift--;
        }

        png_write_row(png_ptr, row_pointer);  
        free(row_pointer);  
    }

    png_write_end(png_ptr, info_ptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    free(buffer); 

    return 0;
}