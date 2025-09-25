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
void display_text();

#endif // MAIN_H