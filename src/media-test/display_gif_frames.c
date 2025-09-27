
#include "../../main.h"

int display_gif_setup(MatrixContext *mctx, GifContext *gctx, const char *gif_filename) {
    if (gctx->frames != NULL) {
        return 1;
    }

    MagickWandGenesis();

    if (!load_gif_frames(gif_filename, &gctx->frames, &gctx->frame_count)) {
        printf("Failed to load GIF :(\n");
        MagickWandTerminus();
        return 0;
    }

    printf("Cool coooool, loaded %d frames from GIF\n", gctx->frame_count);

    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL) {
        printf("MatrixContext not initialized.\n");
        return 0;
    }

    gctx->current_frame = 0;
    return 1;
}

void display_gif_update(MatrixContext *mctx, GifContext *gctx) {
    if (mctx == NULL || gctx == NULL) {
        return;
    }
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL || gctx->frames == NULL || gctx->frame_count == 0) {
        return;
    }

    GifFrame *current = &gctx->frames[gctx->current_frame];

    for (int y = 0; y < mctx->height; ++y) {
        for (int x = 0; x < mctx->width; ++x) {
            int pixel_idx = (y * mctx->width + x) * 3;
            int red = current->pixel_data[pixel_idx];
            int green = current->pixel_data[pixel_idx + 1];
            int blue = current->pixel_data[pixel_idx + 2];
            led_canvas_set_pixel(mctx->offscreen_canvas, x, y, red, green, blue);
        }
    }

    gctx->current_frame = (gctx->current_frame + 1) % gctx->frame_count;
}
