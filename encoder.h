#ifndef ENCODER_H
#define ENCODER_H

#include <png.h>
#include <assert.h>
extern png_structp png_ptr;
extern png_infop info_ptr;

int libpng_init();
int png_get_input_bytes();
#endif