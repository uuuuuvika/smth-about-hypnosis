#ifndef MAIN_H
#define MAIN_H

#include <wand/MagickWand.h>

#include <led-matrix-c.h>
#include "stb_image.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    unsigned char *pixel_data;
    int width;
    int height;
    int delay;
} GifFrame;

typedef struct {
    GifFrame *frames;
    char *filename;
    int frame_count;
    int duration;
} GifAsPlaylistEntry;

typedef struct {
    char *filename;
    int duration; 
} PlaylistEntry;

typedef struct {
    PlaylistEntry *entries;
    int entry_count;
    int current_index;
} Playlist;

int load_gif_frames(const char *filename, GifFrame **frames, int *frame_count);
int load_text(const char *filename, char **text);

#endif // MAIN_H