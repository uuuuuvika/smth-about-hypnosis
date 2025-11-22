#include "../../../main.h"

void create_wave_gif(GifFrame **frames, int *frame_count, MatrixContext *mctx)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg = NewPixelWand(), *fg = NewPixelWand();
    PixelSetColor(bg, "black");
    PixelSetColor(fg, "white");

    *frame_count = NUM_FRAMES;
    *frames = malloc(sizeof(GifFrame) * NUM_FRAMES);

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        MagickNewImage(wand, mctx->width, mctx->height, bg);
        DrawSetStrokeColor(draw, fg);
        DrawSetStrokeWidth(draw, 2);
        DrawSetFillOpacity(draw, 0);

        double phase = (double)f / NUM_FRAMES * M_PI * 2.0;

        for (int x = 0; x < mctx->width - 1; x++) {
            double y1 = mctx->height / 2.0 + sin((x * 0.1) + phase) * 10;
            double y2 = mctx->height / 2.0 + sin(((x + 1) * 0.1) + phase) * 10;
            DrawLine(draw, x, y1, x + 1, y2);
        }

        MagickDrawImage(wand, draw);
        MagickSetImageType(wand, GrayscaleType);

        (*frames)[f].width = mctx->width;
        (*frames)[f].height = mctx->height;
        (*frames)[f].delay = FRAME_DELAY;
        (*frames)[f].pixel_data = malloc(mctx->width * mctx->height);
        MagickExportImagePixels(wand, 0, 0, mctx->width, mctx->height,
                                "I", CharPixel, (*frames)[f].pixel_data);
        DestroyDrawingWand(draw);
        MagickRemoveImage(wand);
    }
    DestroyPixelWand(fg);
    DestroyPixelWand(bg);
    DestroyMagickWand(wand);
}
