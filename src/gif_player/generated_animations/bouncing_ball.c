#include "../../../main.h"

void create_bouncing_ball_gif(GifFrame **frames, int *frame_count)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg = NewPixelWand(), *fg = NewPixelWand();
    PixelSetColor(bg, "black");
    PixelSetColor(fg, "white");

    *frame_count = NUM_FRAMES;
    *frames = malloc(sizeof(GifFrame) * NUM_FRAMES);

    double ball_radius = 6.0;

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        MagickNewImage(wand, MATRIX_WIDTH, MATRIX_HEIGHT, bg);
        DrawSetFillColor(draw, fg);

        double t = (double)f / NUM_FRAMES;
        // Ball bounces using abs(sin) for smooth bounce
        double x = 10 + t * (MATRIX_WIDTH - 20);
        double bounce = fabs(sin(t * M_PI * 4));
        double y = MATRIX_HEIGHT - ball_radius - bounce * (MATRIX_HEIGHT - ball_radius * 2 - 2);

        DrawCircle(draw, x, y, x + ball_radius, y);

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
    DestroyPixelWand(fg);
    DestroyPixelWand(bg);
    DestroyMagickWand(wand);
}