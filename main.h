#ifndef MAIN_H
#define MAIN_H

//#include <MagickWand/MagickWand.h> //for Mac
#include <wand/MagickWand.h> //for Linux

#include <led-matrix-c.h>
#include "led-matrix-c.h"
#include "stb_image.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct
{
    unsigned char *pixel_data;
    int width;
    int height;
    int delay;
} GifFrame;
typedef struct {
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width;
    int height;
} MatrixContext;
typedef struct {
    GifFrame *frames;
    int frame_count;
    int current_frame;
    int black_threshold;
    const char **playlist;
    int playlist_count;
    const char *current_path;
    int loops_remaining;
} GifContext;

typedef struct {
    struct LedFont *font;
    int font_width;
    int font_baseline;
    struct Color color;
    char *text;
    int x_orig;
    int y_orig;
    int x;
    int y;
    int letter_spacing;
} Text;

int load_gif_frames(const char *filename, GifFrame **frames, int *frame_count);
char *load_text_from_file(const char *filename);
void display_text();

int matrix_setup(MatrixContext *ctx);

int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b);
void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b, int overdraw_left);

int text_setup(MatrixContext *mctx, Text *top, Text *bottom);
void text_update(MatrixContext *mctx, Text *top, Text *bottom);

#endif