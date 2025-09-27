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
            if (avg_brightness(ra, ga, ba) > a->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, ra, ga, ba);
            }

            // Draw B
            int rb = fb->pixel_data[idx];
            int gb = fb->pixel_data[idx + 1];
            int bb = fb->pixel_data[idx + 2];
            if (avg_brightness(rb, gb, bb) > b->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, rb, gb, bb);
            }
        }
    }

    a->current_frame = (a->current_frame + 1) % a->frame_count;
    b->current_frame = (b->current_frame + 1) % b->frame_count;
}

// --- Playlist-based dual-gif playback ---
static inline void free_gif_frames(GifContext *ctx) {
    if (ctx == NULL || ctx->frames == NULL) return;
    // Frames' pixel_data allocated per frame; free them individually
    for (int i = 0; i < ctx->frame_count; ++i) {
        if (ctx->frames[i].pixel_data) {
            free(ctx->frames[i].pixel_data);
            ctx->frames[i].pixel_data = NULL;
        }
    }
    free(ctx->frames);
    ctx->frames = NULL;
    ctx->frame_count = 0;
}

static inline int rand_range(int min_inclusive, int max_inclusive) {
    if (max_inclusive <= min_inclusive) return min_inclusive;
    int span = max_inclusive - min_inclusive + 1;
    return min_inclusive + (rand() % span);
}

static int load_random_gif_for_layer(GifContext *layer) {
    if (layer->playlist == NULL || layer->playlist_count <= 0) {
        return 0;
    }
    int idx = rand_range(0, layer->playlist_count - 1);
    const char *path = layer->playlist[idx];
    if (path == NULL) return 0;

    // Free existing
    free_gif_frames(layer);

    if (!load_gif_frames(path, &layer->frames, &layer->frame_count)) {
        printf("Failed to load GIF for layer: %s\n", path);
        return 0;
    }
    layer->current_frame = 0;
    layer->current_path = path;
    layer->loops_remaining = rand_range(10, 20);
    return 1;
}

int display_gifs_playlist_setup(MatrixContext *mctx, GifContext *a, GifContext *b, const char **paths, int count) {
    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL) {
        printf("MatrixContext not initialized.\n");
        return 0;
    }
    if (paths == NULL || count <= 0) {
        printf("Playlist is empty.\n");
        return 0;
    }

    MagickWandGenesis();

    // Assign playlists to both layers
    a->playlist = paths;
    a->playlist_count = count;
    b->playlist = paths;
    b->playlist_count = count;

    a->black_threshold = 32;
    b->black_threshold = 32;

    // Load initial random gifs for both layers
    if (!load_random_gif_for_layer(a)) return 0;
    if (!load_random_gif_for_layer(b)) return 0;

    printf("Playlist mode: A(%d frames, loops %d) B(%d frames, loops %d)\n",
           a->frame_count, a->loops_remaining, b->frame_count, b->loops_remaining);
    return 1;
}

void display_gifs_playlist_update(MatrixContext *mctx, GifContext *a, GifContext *b) {
    if (mctx == NULL || a == NULL || b == NULL) return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL) return;
    if (a->frames == NULL || a->frame_count == 0) return;
    if (b->frames == NULL || b->frame_count == 0) return;

    GifFrame *fa = &a->frames[a->current_frame];
    GifFrame *fb = &b->frames[b->current_frame];

    for (int y = 0; y < mctx->height; ++y) {
        for (int x = 0; x < mctx->width; ++x) {
            int idx = (y * mctx->width + x) * 3;

            int ra = fa->pixel_data[idx];
            int ga = fa->pixel_data[idx + 1];
            int ba = fa->pixel_data[idx + 2];
            if (avg_brightness(ra, ga, ba) > a->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, ra, ga, ba);
            }

            int rb = fb->pixel_data[idx];
            int gb = fb->pixel_data[idx + 1];
            int bb = fb->pixel_data[idx + 2];
            if (avg_brightness(rb, gb, bb) > b->black_threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, rb, gb, bb);
            }
        }
    }

    // Advance frames
    a->current_frame++;
    if (a->current_frame >= a->frame_count) {
        a->current_frame = 0;
        if (a->loops_remaining > 0) a->loops_remaining--;
        if (a->loops_remaining == 0) {
            if (!load_random_gif_for_layer(a)) {
                // best-effort: keep previous if load failed
                a->loops_remaining = rand_range(10, 20);
            }
        }
    }

    b->current_frame++;
    if (b->current_frame >= b->frame_count) {
        b->current_frame = 0;
        if (b->loops_remaining > 0) b->loops_remaining--;
        if (b->loops_remaining == 0) {
            if (!load_random_gif_for_layer(b)) {
                b->loops_remaining = rand_range(10, 20);
            }
        }
    }
}