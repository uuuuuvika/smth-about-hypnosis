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
void display_text();

// Matrix context shared across modules
typedef struct {
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width;
    int height;
} MatrixContext;

// Matrix lifecycle owned by main
int matrix_setup(MatrixContext *ctx);

// GIF module context
typedef struct {
    GifFrame *frames;
    int frame_count;
    int current_frame;
    int black_threshold;      // 0..255, lower means more pixels become transparent
    unsigned char bg_r, bg_g, bg_b; // background color to fill before drawing
} GifContext;

// GIF split-phase API using contexts
int display_gif_setup(MatrixContext *mctx, GifContext *gctx, const char *gif_filename);
void display_gif_update(MatrixContext *mctx, GifContext *gctx);

int display_two_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b, const char *file_a, const char *file_b);
void display_two_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b);

#endif // MAIN_H