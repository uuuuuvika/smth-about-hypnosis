#include "../../main.h"

static inline int avg_brightness(int r, int g, int b) {
    return (r + g + b) / 3;
}

int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b, const char *file_a, const char *file_b) {
    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL) {
        printf("MatrixContext not initialized.\n");
        return 0;
    }

    MagickWandGenesis();

    if (!load_gif_frames(file_a, &a->frames, &a->frame_count)) {
        printf("Failed to load GIF A: %s\n", file_a);
        MagickWandTerminus();
        return 0;
    }
    if (!load_gif_frames(file_b, &b->frames, &b->frame_count)) {
        printf("Failed to load GIF B: %s\n", file_b);
        MagickWandTerminus();
        return 0;
    }

    a->current_frame = 0;
    b->current_frame = 0;

    a->bg_r = 0; a->bg_g = 0; a->bg_b = 255;
    a->black_threshold = 32;
    b->black_threshold = 32;

    printf("Loaded %d frames (A) and %d frames (B)\n", a->frame_count, b->frame_count);
    return 1;
}

void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b) {
    if (mctx == NULL || a == NULL || b == NULL) return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL) return;
    if (a->frames == NULL || a->frame_count == 0) return;
    if (b->frames == NULL || b->frame_count == 0) return;


    GifFrame *fa = &a->frames[a->current_frame];
    GifFrame *fb = &b->frames[b->current_frame];

    for (int y = 0; y < mctx->height; ++y) {
        for (int x = 0; x < mctx->width; ++x) {
            int idx = (y * mctx->width + x) * 3;

            // Draw A
            int ra = fa->pixel_data[idx];
            int ga = fa->pixel_data[idx + 1];
            int ba = fa->pixel_data[idx + 2];
            if ((ra + ga + ba) / 3 > a->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, ra, ga, ba);
            }

            // Draw B
            int rb = fb->pixel_data[idx];
            int gb = fb->pixel_data[idx + 1];
            int bb = fb->pixel_data[idx + 2];
            if ((rb + gb + bb) / 3 > b->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, rb, gb, bb);
            }
        }
    }

    a->current_frame = (a->current_frame + 1) % a->frame_count;
    b->current_frame = (b->current_frame + 1) % b->frame_count;
}