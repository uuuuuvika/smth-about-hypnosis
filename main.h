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
#include <signal.h>

#define MATRIX_WIDTH 128
#define MATRIX_HEIGHT 32
#define NUM_FRAMES 30
#define FRAME_DELAY 4
#define M_PI 3.14159265358979323846
#define MAX_LOOPS 10
#define MIN_LOOPS 5

typedef struct {
    double time;
    int num_particles;
} ParticleAnimation;

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

typedef struct
{
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width;
    int height;
} MatrixContext;

int matrix_setup(MatrixContext *ctx);
void overdraw_half(struct LedCanvas *canvas, int width, int height, int left_half);
int rand_range(int min, int max);

char *load_text_from_file(const char *filename);
int text_setup(MatrixContext *mctx, Text *top, Text *bottom);
void text_update(MatrixContext *mctx, Text *top, Text *bottom);

void particle_animation_init(ParticleAnimation *anim);
void particle_animation_draw(ParticleAnimation *anim, MatrixContext *mctx, int x_offset, int width);

#endif