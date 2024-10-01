#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "encoder.h"

int main(int argc, char* argv[])
{
    if(argc != 6)
    {
        printf("Usage: main [input] [output] [buffersize] [width] [height]");
        return 1;
    }

    char* input = argv[1];
    char* output = argv[2];
    int buffersize = atoi(argv[3]);
    int width = atoi(argv[4]);
    int height = atoi(argv[5]);

    int bufferpos = 0, buffershift = 7;

    if(libpng_init(output, width, height) != 0)
    {
        fprintf(stderr, "There was an error with libpng!\n");
        return -1;
    }

    char* buffer = malloc(buffersize);
    memset(buffer, 0, buffersize); 

    if(png_get_input_bytes(input, buffer, buffersize) != 0)
    {
        fprintf(stderr, "There was an error reading %s or file is empty!\n", input);
        return -1;
    }

    // if there are more bytes of data in the input file than there are
    // in the available space in the buffer return...

    for(int row = 0; row < height; row++)
    {
        png_bytep row_pointer = (png_bytep) malloc(width * sizeof(png_byte));
        memset(row_pointer, 0, width * sizeof(png_byte));

        for(int col = 0; col < width; col++)
        {
            if(bufferpos >= buffersize) continue;

            if(buffershift < 0)
            {
                bufferpos++;
                buffershift = 7;
            }

            if(((buffer[bufferpos] >> buffershift) & 1) == 1)
                row_pointer[col] = 255;
            else
                row_pointer[col] = 0;

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
