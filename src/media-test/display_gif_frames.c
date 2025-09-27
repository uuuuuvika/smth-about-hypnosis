#include "../../main.h"
#include <time.h>
#include <dirent.h>
#include <strings.h>
#include "gif_playlist.h"

const int max_loops = 10;
const int min_loops = 5;

static inline int avg_brightness(int r, int g, int b) {
    return (r + g + b) / 3;
}

static void show_loading_text(MatrixContext *mctx)
{
    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL) return;
    const char *msg = "loading media";
    const char *font_file = "fonts/unifont.bdf";
    struct LedFont *font = load_font(font_file);
    if (font == NULL) {
        led_canvas_fill(mctx->offscreen_canvas, 0, 0, 0);
        mctx->offscreen_canvas = led_matrix_swap_on_vsync(mctx->matrix, mctx->offscreen_canvas);
        return;
    }
    int font_width = character_width_font(font, 'W');
    int baseline = baseline_font(font);
    int text_width = (int)strlen(msg) * font_width;
    int x = (mctx->width - text_width) / 2;
    if (x < 0) x = 0;
    int y = baseline + (mctx->height - baseline) / 2;

    led_canvas_fill(mctx->offscreen_canvas, 0, 0, 0);
    draw_text(mctx->offscreen_canvas, font, x, y, 255, 255, 255, msg, 1);
    mctx->offscreen_canvas = led_matrix_swap_on_vsync(mctx->matrix, mctx->offscreen_canvas);
    delete_font(font);
}
static inline void draw_frame_to_canvas(MatrixContext *mctx, GifFrame *frame, int threshold, int x_start, int x_end) {
    if (x_start < 0) x_start = 0;
    if (x_end > mctx->width) x_end = mctx->width;
    if (x_start >= x_end) return;
    for (int y = 0; y < mctx->height; ++y) {
        for (int x = x_start; x < x_end; ++x) {
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
    char name[256]; // filename only
} PreloadedGif;

static PreloadedGif s_preloaded[512];
static int s_preloaded_count = 0;
static int s_preloaded_ready = 0;

// Scan BASE_PATH for .gif files and preload all
static int preload_all_gifs() {
    if (s_preloaded_ready) return 1;
    s_preloaded_count = 0;
    DIR *dir = opendir(BASE_PATH);
    if (!dir) {
        perror("opendir(BASE_PATH)");
        return 0;
    }
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL) {
        const char *fname = ent->d_name;
        size_t len = strlen(fname);
        if (len < 5) continue; // need at least 'a.gif'
        if (strcasecmp(fname + len - 4, ".gif") != 0) continue;
        if (s_preloaded_count >= (int)(sizeof(s_preloaded)/sizeof(s_preloaded[0]))) break;

        // Guard against excessively long paths and names
        const size_t full_cap = 512;
        size_t base_len = strlen(BASE_PATH);
        if (base_len + len >= full_cap) {
            printf("Skipping GIF (path too long): %s%s\n", BASE_PATH, fname);
            continue;
        }
        if (len >= sizeof(s_preloaded[s_preloaded_count].name)) {
            printf("Skipping GIF (name too long): %s\n", fname);
            continue;
        }

        char full_path[512];
        // Safe, length-checked build of full path
        snprintf(full_path, sizeof(full_path), "%s%s", BASE_PATH, fname);

        GifFrame *frames = NULL;
        int frame_count = 0;
        if (!load_gif_frames(full_path, &frames, &frame_count)) {
            printf("Error: Could not load %s\n", full_path);
            continue;
        }
        s_preloaded[s_preloaded_count].frames = frames;
        s_preloaded[s_preloaded_count].frame_count = frame_count;
        // Copy filename safely into fixed buffer
        snprintf(s_preloaded[s_preloaded_count].name,
                 sizeof(s_preloaded[s_preloaded_count].name),
                 "%s", fname);
        s_preloaded_count++;
    }
    closedir(dir);
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

    MagickWandGenesis();
    // Seed random number generator
    srand((unsigned int)time(NULL));

    // No static playlist anymore; we'll scan and preload
    a->playlist = NULL;
    a->playlist_count = 0;
    b->playlist = NULL;
    b->playlist_count = 0;

    a->black_threshold = 0;
    b->black_threshold = 32;

    // Show loading notice while preloading from disk
    show_loading_text(mctx);

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

void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b, int overdraw_left) {
    if (mctx == NULL || a == NULL || b == NULL) return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL) return;
    if (a->frames == NULL || a->frame_count == 0) return;
    if (b->frames == NULL || b->frame_count == 0) return;

    GifFrame *fa = &a->frames[a->current_frame];
    GifFrame *fb = &b->frames[b->current_frame];
    int mid = mctx->width / 2;
    int x_start = overdraw_left ? mid : 0;
    int x_end = overdraw_left ? mctx->width : mid;

    // Draw both layers constrained to the selected half
    draw_frame_to_canvas(mctx, fa, a->black_threshold, x_start, x_end);
    draw_frame_to_canvas(mctx, fb, b->black_threshold, x_start, x_end);

    advance_layer(a);
    advance_layer(b);
}