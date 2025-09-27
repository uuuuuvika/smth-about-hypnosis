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
    // Playlist / random playback
    const char **playlist;    // array of file paths
    int playlist_count;       // number of entries in playlist
    const char *current_path; // currently loaded gif path
    int loops_remaining;      // remaining full loops before switching gif
} GifContext;

int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b, const char *file_a, const char *file_b);
void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b);

// Playlist-based API (layers A and B pick random gifs and loop 10..20 times)
int display_gifs_playlist_setup(MatrixContext *mctx, GifContext *a, GifContext *b, const char **paths, int count);
void display_gifs_playlist_update(MatrixContext *mctx, GifContext *a, GifContext *b);

#endif // MAIN_H