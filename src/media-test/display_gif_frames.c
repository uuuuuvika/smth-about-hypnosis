#include "../../main.h"
#include <time.h>
#include "gif_playlist.h"

const int max_loops = 10;
const int min_loops = 5;

static inline int avg_brightness(int r, int g, int b) {
    return (r + g + b) / 3;
}

static inline void draw_frame_to_canvas(MatrixContext *mctx, GifFrame *frame, int threshold) {
    for (int y = 0; y < mctx->height; ++y) {
        for (int x = 0; x < mctx->width; ++x) {
            int idx = (y * mctx->width + x) * 3;
            int r = frame->pixel_data[idx];
            int g = frame->pixel_data[idx + 1];
            int b = frame->pixel_data[idx + 2];
            if (avg_brightness(r, g, b) > threshold) {
                led_canvas_set_pixel(mctx->offscreen_canvas, x, y, r, g, b);
            }
        }
    }
}

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

// Preload storage for all GIFs so runtime switching doesn't trigger disk I/O
typedef struct {
    GifFrame *frames;
    int frame_count;
    const char *name; // filename from playlist (without base path)
} PreloadedGif;

static PreloadedGif s_preloaded[GIF_PLAYLIST_COUNT];
static int s_preloaded_count = 0;
static int s_preloaded_ready = 0;

static int preload_all_gifs() {
    if (s_preloaded_ready) return 1;
    s_preloaded_count = 0;
    for (int i = 0; i < (int)GIF_PLAYLIST_COUNT; ++i) {
        const char *name = GIF_PLAYLIST[i];
        char full_path[512];
        snprintf(full_path, sizeof(full_path), "%s%s", BASE_PATH, name);
        GifFrame *frames = NULL;
        int frame_count = 0;
        if (!load_gif_frames(full_path, &frames, &frame_count)) {
            printf("Error: Could not load %s\n", full_path);
            continue;
        }
        s_preloaded[s_preloaded_count].frames = frames;
        s_preloaded[s_preloaded_count].frame_count = frame_count;
        s_preloaded[s_preloaded_count].name = name;
        s_preloaded_count++;
    }
    if (s_preloaded_count == 0) return 0;
    s_preloaded_ready = 1;
    return 1;
}

static int load_random_gif_for_layer(GifContext *layer) {
    if (!s_preloaded_ready || s_preloaded_count <= 0) {
        return 0;
    }
    int idx = rand_range(0, s_preloaded_count - 1);
    PreloadedGif *sel = &s_preloaded[idx];
    if (sel->frames == NULL || sel->frame_count <= 0) return 0;
    // Assign pointers to preloaded frames (do not free on switch)
    layer->frames = sel->frames;
    layer->frame_count = sel->frame_count;
    layer->current_frame = 0;
    layer->current_path = sel->name;
    layer->loops_remaining = rand_range(min_loops, max_loops);
    return 1;
}

static void advance_layer(GifContext *layer) {
    layer->current_frame++;
    if (layer->current_frame >= layer->frame_count) {
        layer->current_frame = 0;
        if (layer->loops_remaining > 0) layer->loops_remaining--;
        if (layer->loops_remaining == 0) {
            if (!load_random_gif_for_layer(layer)) {
                // best-effort fallback: keep current and reset loops
                layer->loops_remaining = rand_range(10, 20);
            }
        }
    }
}

int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b) {
    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL) {
        printf("MatrixContext not initialized.\n");
        return 0;
    }
    if (GIF_PLAYLIST == NULL || GIF_PLAYLIST_COUNT <= 0) {
        printf("Playlist is empty.\n");
        return 0;
    }

    MagickWandGenesis();
    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Assign playlists to both layers
    a->playlist = GIF_PLAYLIST;
    a->playlist_count = GIF_PLAYLIST_COUNT;
    b->playlist = GIF_PLAYLIST;
    b->playlist_count = GIF_PLAYLIST_COUNT;

    a->black_threshold = 0;
    b->black_threshold = 32;

    // Preload all GIFs once to avoid runtime load glitches
    if (!preload_all_gifs()) {
        printf("Failed to preload GIFs.\n");
        return 0;
    }

    // Pick initial random gifs for both layers from preloaded set
    if (!load_random_gif_for_layer(a)) return 0;
    if (!load_random_gif_for_layer(b)) return 0;

    return 1;
}

void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b) {
    if (mctx == NULL || a == NULL || b == NULL) return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL) return;
    if (a->frames == NULL || a->frame_count == 0) return;
    if (b->frames == NULL || b->frame_count == 0) return;

    GifFrame *fa = &a->frames[a->current_frame];
    GifFrame *fb = &b->frames[b->current_frame];

    draw_frame_to_canvas(mctx, fa, a->black_threshold);
    draw_frame_to_canvas(mctx, fb, b->black_threshold);

    advance_layer(a);
    advance_layer(b);
}