#include <stdlib.h>
#include <math.h>
#include <MagickWand/MagickWand.h>

#define MATRIX_WIDTH 128
#define MATRIX_HEIGHT 32
#define NUM_FRAMES 60
#define FRAME_DELAY 3 

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

typedef struct {
    int width;
    int height;
    int delay;
    unsigned char *pixel_data;
} GifFrame;

void draw_wobbly_circle(DrawingWand *draw, double time, double thickness)
{
    const int segments = 64;
    const double base_radius = (MATRIX_HEIGHT / 2.0) * 0.7;
    const double center_x = MATRIX_WIDTH / 2.0;
    const double center_y = MATRIX_HEIGHT / 2.0;

    PointInfo *outer_points = malloc((segments + 1) * sizeof(PointInfo));
    PointInfo *inner_points = malloc((segments + 1) * sizeof(PointInfo));

    for (int i = 0; i <= segments; i++) {
        double angle = ((double)i / segments) * M_PI * 2.0;

        // Wobble calculation (scaled down for small display)
        double wobble = 
            sin(angle * 3.0 + time * 2.0) * 3.0 +
            sin(angle * 5.0 - time * 1.5) * 2.0 +
            sin(angle * 2.0 + time * 3.0) * 1.5;

        double radius = base_radius + wobble;

        double cos_a = cos(angle);
        double sin_a = sin(angle);

        double inner_r = radius - thickness / 2.0;
        double outer_r = radius + thickness / 2.0;

        inner_points[i].x = center_x + cos_a * inner_r;
        inner_points[i].y = center_y + sin_a * inner_r;

        outer_points[i].x = center_x + cos_a * outer_r;
        outer_points[i].y = center_y + sin_a * outer_r;
    }

    // Draw as filled polygon (outer path then inner path reversed)
    // Using triangle strip approach: draw segment by segment
    for (int i = 0; i < segments; i++) {
        DrawPathStart(draw);
        DrawPathMoveToAbsolute(draw, outer_points[i].x, outer_points[i].y);
        DrawPathLineToAbsolute(draw, outer_points[i + 1].x, outer_points[i + 1].y);
        DrawPathLineToAbsolute(draw, inner_points[i + 1].x, inner_points[i + 1].y);
        DrawPathLineToAbsolute(draw, inner_points[i].x, inner_points[i].y);
        DrawPathClose(draw);
        DrawPathFinish(draw);
    }

    free(outer_points);
    free(inner_points);
}

void create_wobbly_circle_gif(GifFrame **frames, int *frame_count)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg_color = NewPixelWand();
    PixelWand *fg_color = NewPixelWand();

    PixelSetColor(bg_color, "black");
    PixelSetColor(fg_color, "white");

    *frame_count = NUM_FRAMES;
    *frames = malloc(sizeof(GifFrame) * NUM_FRAMES);

    double thickness = 2.0;  // Adjust for LED matrix visibility

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        
        MagickNewImage(wand, MATRIX_WIDTH, MATRIX_HEIGHT, bg_color);

        DrawSetFillColor(draw, fg_color);
        DrawSetStrokeColor(draw, fg_color);
        DrawSetStrokeWidth(draw, 0);

        double time = (double)f / NUM_FRAMES * M_PI * 2.0;
        draw_wobbly_circle(draw, time, thickness);

        MagickDrawImage(wand, draw);
        MagickSetImageType(wand, GrayscaleType);

        (*frames)[f].width = MATRIX_WIDTH;
        (*frames)[f].height = MATRIX_HEIGHT;
        (*frames)[f].delay = FRAME_DELAY;
        (*frames)[f].pixel_data = malloc(MATRIX_WIDTH * MATRIX_HEIGHT);

        MagickExportImagePixels(wand, 0, 0, MATRIX_WIDTH, MATRIX_HEIGHT,
                                "I", CharPixel, (*frames)[f].pixel_data);

        DestroyDrawingWand(draw);
        MagickRemoveImage(wand);
    }

    DestroyPixelWand(fg_color);
    DestroyPixelWand(bg_color);
    DestroyMagickWand(wand);
}

void free_wobbly_circle_gif(GifFrame *frames, int frame_count)
{
    for (int i = 0; i < frame_count; i++) {
        free(frames[i].pixel_data);
    }
    free(frames);
}