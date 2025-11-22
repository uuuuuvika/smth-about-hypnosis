
#include "../../../main.h"

static void draw_wobbly_circle(DrawingWand *draw, double time, double thickness, MatrixContext *mctx)
{
    const int segments = 64;
    const double base_radius = (mctx->height / 2.0) * 0.7;
    const double cx = mctx->width / 2.0;
    const double cy = mctx->height / 2.0;

    PointInfo *outer = malloc((segments + 1) * sizeof(PointInfo));
    PointInfo *inner = malloc((segments + 1) * sizeof(PointInfo));

    for (int i = 0; i <= segments; i++) {
        double angle = ((double)i / segments) * M_PI * 2.0;
        double wobble = sin(angle * 3.0 + time * 2.0) * 3.0 +
                        sin(angle * 5.0 - time * 1.5) * 2.0 +
                        sin(angle * 2.0 + time * 3.0) * 1.5;
        double r = base_radius + wobble;
        double c = cos(angle), s = sin(angle);
        inner[i].x = cx + c * (r - thickness / 2.0);
        inner[i].y = cy + s * (r - thickness / 2.0);
        outer[i].x = cx + c * (r + thickness / 2.0);
        outer[i].y = cy + s * (r + thickness / 2.0);
    }

    for (int i = 0; i < segments; i++) {
        DrawPathStart(draw);
        DrawPathMoveToAbsolute(draw, outer[i].x, outer[i].y);
        DrawPathLineToAbsolute(draw, outer[i + 1].x, outer[i + 1].y);
        DrawPathLineToAbsolute(draw, inner[i + 1].x, inner[i + 1].y);
        DrawPathLineToAbsolute(draw, inner[i].x, inner[i].y);
        DrawPathClose(draw);
        DrawPathFinish(draw);
    }
    free(outer);
    free(inner);
}

void create_wobbly_circle_gif(GifFrame **frames, int *frame_count, MatrixContext *mctx)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg = NewPixelWand(), *fg = NewPixelWand();

    PixelSetColor(bg, "black");
    PixelSetColor(fg, "white");

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        MagickNewImage(wand, mctx->width, mctx->height, bg);
        MagickSetImageDelay(wand, FRAME_DELAY);
        
        DrawSetFillColor(draw, fg);
        DrawSetStrokeColor(draw, fg);

        double time = (double)f / NUM_FRAMES * M_PI * 2.0;
        draw_wobbly_circle(draw, time, 2.0, mctx);

        MagickDrawImage(wand, draw);
        DestroyDrawingWand(draw);
    }

    MagickWand *coalesced_wand = MagickCoalesceImages(wand);
    if (coalesced_wand == NULL) {
        printf("Error: Could not coalesce wobbly circle frames\n");
        DestroyMagickWand(wand);
        DestroyPixelWand(fg);
        DestroyPixelWand(bg);
        *frames = NULL;
        *frame_count = 0;
        return;
    }

    *frame_count = MagickGetNumberImages(coalesced_wand);
    *frames = malloc(sizeof(GifFrame) * (*frame_count));
    
    MagickResetIterator(coalesced_wand);
    
    int i = 0;
    while (MagickNextImage(coalesced_wand) != MagickFalse) {
        GifFrame *current_frame = &(*frames)[i];

        MagickSetImageType(coalesced_wand, GrayscaleType);

        current_frame->width = MagickGetImageWidth(coalesced_wand);
        current_frame->height = MagickGetImageHeight(coalesced_wand);
        current_frame->delay = MagickGetImageDelay(coalesced_wand);
        if (current_frame->delay == 0) {
            current_frame->delay = FRAME_DELAY;
        }

        // RGB format to match load_gif_frames
        current_frame->pixel_data = malloc(current_frame->width * current_frame->height * 3);
        MagickExportImagePixels(coalesced_wand, 0, 0, 
                                current_frame->width, current_frame->height,
                                "RGB", CharPixel, current_frame->pixel_data);
        i++;
    }

    DestroyMagickWand(coalesced_wand);
    DestroyMagickWand(wand);
    DestroyPixelWand(fg);
    DestroyPixelWand(bg);
}