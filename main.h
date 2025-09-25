#ifndef MAIN_H
#define MAIN_H

#include <MagickWand/MagickWand.h> //for Mac
// #include <wand/MagickWand.h> //for Linux

#include <led-matrix-c.h>
#include "stb_image.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const struct RGBLedMatrixOptions
{
    int rows;
    int cols;
    int chain_length;
    const char *hardware_mapping;
    bool disable_hardware_pulsing;
    int brightness;
} options = {
    .rows = 32,
    .cols = 64,
    .chain_length = 2,
    .hardware_mapping = "adafruit-hat",
    .disable_hardware_pulsing = true,
    .brightness = 50
};

typedef struct
{
    unsigned char *pixel_data;
    int width;
    int height;
    int delay;
} GifFrame;

typedef struct
{
    GifFrame *frames;
    char *filename;
    int frame_count;
    int duration;
} GifAsPlaylistEntry;

typedef struct
{
    char *filename;
    int duration;
} PlaylistEntry;

typedef struct
{
    PlaylistEntry *entries;
    int entry_count;
    int current_index;
} Playlist;

int load_gif_frames(const char *filename, GifFrame **frames, int *frame_count);
char *load_text_from_file(const char *filename);
void display_gif_on_matrix(const char *gif_filename);

#endif // MAIN_H