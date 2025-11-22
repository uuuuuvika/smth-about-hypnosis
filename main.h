#ifndef MAIN_H
#define MAIN_H

#include <MagickWand/MagickWand.h>

#include <led-matrix-c.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <strings.h>
#include <math.h>

#define BASE_PATH "assets/gifs/"
#define MATRIX_WIDTH 128
#define MATRIX_HEIGHT 32
#define NUM_FRAMES 60
#define FRAME_DELAY 3
#define M_PI 3.14159265358979323846

typedef void (*GifCreator)(GifFrame **frames, int *frame_count, MatrixContext *mctx);
    
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
    int frame_count;
    char name[256];
} PreloadedGif;

typedef struct
{
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width;
    int height;
} MatrixContext;

typedef struct
{
    GifFrame *frames;
    int frame_count;
    int current_frame;
    int black_threshold;
    const char **playlist;
    int playlist_count;
    const char *current_path;
    int loops_remaining;
} GifContext;

typedef struct
{
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

typedef struct {
    const char *name;
    GifCreator creator;
} GifAnimation;

static GifAnimation animations[] = {
    {"wobbly_circle", create_wobbly_circle_gif},
    {"bouncing_ball", create_bouncing_ball_gif},
    {"wave",          create_wave_gif},
    // Add new entries: {"your_animation_name", create_your_animation_gif},
    {NULL, NULL}  // Terminator
};

static PreloadedGif s_preloaded[512];
static int s_preloaded_count = 0;
static int s_preloaded_ready = 0;
const int max_loops = 10;
const int min_loops = 5;

int matrix_setup(MatrixContext *ctx);
void show_loading_text(MatrixContext *mctx);

void overdraw_half(struct LedCanvas *canvas, int width, int height, int left_half);
int rand_range(int min, int max);

int preload_all_gifs();
int load_random_gif_for_layer(GifContext *layer);
int load_gif_frames(const char *filename, GifFrame **frames, int *frame_count);
char *load_text_from_file(const char *filename);

int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b);
void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b, int half_mode);

void free_all_gif_frames(GifContext *ctx);
void free_gif_frames(GifFrame *frames, int frame_count);

int text_setup(MatrixContext *mctx, Text *top, Text *bottom);
void text_update(MatrixContext *mctx, Text *top, Text *bottom);

void create_wobbly_circle_gif(GifFrame **frames, int *frame_coun, MatrixContext *mctx);
void create_bouncing_ball_gif(GifFrame **frames, int *frame_count, MatrixContext *mctx);
void create_wave_gif(GifFrame **frames, int *frame_count, MatrixContext *mctx);

#endif