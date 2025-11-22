#include "main.h"

#include <stdlib.h>
#include <math.h>
#include <string.h>
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

// Function pointer type for GIF creators
typedef void (*GifCreator)(GifFrame **frames, int *frame_count);

// Registry entry for animations
typedef struct {
    const char *name;
    GifCreator creator;
} GifAnimation;

// Forward declarations
void create_wobbly_circle_gif(GifFrame **frames, int *frame_count);
void create_bouncing_ball_gif(GifFrame **frames, int *frame_count);
void create_wave_gif(GifFrame **frames, int *frame_count);
void free_gif_frames(GifFrame *frames, int frame_count);

// ============ ANIMATION REGISTRY ============
// Add your new animations here!
static GifAnimation animations[] = {
    {"wobbly_circle", create_wobbly_circle_gif},
    {"bouncing_ball", create_bouncing_ball_gif},
    {"wave",          create_wave_gif},
    // Add new entries: {"your_animation_name", create_your_animation_gif},
    {NULL, NULL}  // Terminator
};

// Get animation by name
GifCreator get_animation(const char *name)
{
    for (int i = 0; animations[i].name != NULL; i++) {
        if (strcmp(animations[i].name, name) == 0) {
            return animations[i].creator;
        }
    }
    return NULL;
}

// List all available animations
void list_animations(void)
{
    printf("Available animations:\n");
    for (int i = 0; animations[i].name != NULL; i++) {
        printf("  - %s\n", animations[i].name);
    }
}

// ============ WOBBLY CIRCLE ============
void draw_wobbly_circle(DrawingWand *draw, double time, double thickness)
{
    const int segments = 64;
    const double base_radius = (MATRIX_HEIGHT / 2.0) * 0.7;
    const double cx = MATRIX_WIDTH / 2.0;
    const double cy = MATRIX_HEIGHT / 2.0;

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

void create_wobbly_circle_gif(GifFrame **frames, int *frame_count)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg = NewPixelWand(), *fg = NewPixelWand();
    PixelSetColor(bg, "black");
    PixelSetColor(fg, "white");

    *frame_count = NUM_FRAMES;
    *frames = malloc(sizeof(GifFrame) * NUM_FRAMES);

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        MagickNewImage(wand, MATRIX_WIDTH, MATRIX_HEIGHT, bg);
        DrawSetFillColor(draw, fg);
        DrawSetStrokeColor(draw, fg);

        double time = (double)f / NUM_FRAMES * M_PI * 2.0;
        draw_wobbly_circle(draw, time, 2.0);

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

// ============ BOUNCING BALL (NEW EXAMPLE) ============
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

// ============ WAVE (ANOTHER EXAMPLE) ============
void create_wave_gif(GifFrame **frames, int *frame_count)
{
    MagickWand *wand = NewMagickWand();
    PixelWand *bg = NewPixelWand(), *fg = NewPixelWand();
    PixelSetColor(bg, "black");
    PixelSetColor(fg, "white");

    *frame_count = NUM_FRAMES;
    *frames = malloc(sizeof(GifFrame) * NUM_FRAMES);

    for (int f = 0; f < NUM_FRAMES; f++) {
        DrawingWand *draw = NewDrawingWand();
        MagickNewImage(wand, MATRIX_WIDTH, MATRIX_HEIGHT, bg);
        DrawSetStrokeColor(draw, fg);
        DrawSetStrokeWidth(draw, 2);
        DrawSetFillOpacity(draw, 0);

        double phase = (double)f / NUM_FRAMES * M_PI * 2.0;

        // Draw sine wave
        for (int x = 0; x < MATRIX_WIDTH - 1; x++) {
            double y1 = MATRIX_HEIGHT / 2.0 + sin((x * 0.1) + phase) * 10;
            double y2 = MATRIX_HEIGHT / 2.0 + sin(((x + 1) * 0.1) + phase) * 10;
            DrawLine(draw, x, y1, x + 1, y2);
        }

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

// ============ CLEANUP ============
void free_gif_frames(GifFrame *frames, int frame_count)
{
    for (int i = 0; i < frame_count; i++) {
        free(frames[i].pixel_data);
    }
    free(frames);
}

// ============ USAGE EXAMPLE ============

// int main(int argc, char *argv[])
// {
//     MagickWandGenesis();
    
//     GifFrame *frames;
//     int frame_count;
    
//     // Option 1: Call directly
//     create_bouncing_ball_gif(&frames, &frame_count);
    
//     // Option 2: Use registry by name
//     GifCreator creator = get_animation("wave");
//     if (creator) {
//         creator(&frames, &frame_count);
//     }
    
//     // Use frames with your LED matrix...
    
//     free_gif_frames(frames, frame_count);
//     MagickWandTerminus();
//     return 0;
// }

int main(int argc, char **argv)
{
    MatrixContext mctx = {0};
    GifContext ga = {0};
    GifContext gb = {0};
    Text text = {0};
    Text bottom_text = {0};
    // Text *multi_text[] = {
    //     &text,
    //     &bottom_text};

    if (!matrix_setup(&mctx))
    {
        printf("Failed to setup matrix.\n");
        return 1;
    }
    if (!display_gifs_setup(&mctx, &ga, &gb))
    {
        printf("Failed to setup dual GIF rendering.\n");
        led_matrix_delete(mctx.matrix);
        return 1;
    }
    if (!text_setup(&mctx, &text, &bottom_text))
    {
        printf("Failed to setup text rendering.\n");
        led_matrix_delete(mctx.matrix);
        free_gif_frames(&ga);
        free_gif_frames(&gb);
        return 1;
    }

    int mode = 0;
    int mode_min_frames = 180;
    int mode_max_frames = 1600;
    int frames_in_mode = 0;
    int mode_frames = rand_range(mode_min_frames, mode_max_frames);

    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);

        switch (mode)
        {
        case 0: // Left GIF only, Right Text only
            text_update(&mctx, &text, &bottom_text);
            overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1);
            display_gifs_update(&mctx, &ga, &gb, 0);
            break;
        case 1: // Left Text only, Right GIF only
            text_update(&mctx, &text, &bottom_text);
            overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 0);
            display_gifs_update(&mctx, &ga, &gb, 1);
            break;
        case 2: // GIF on both sides, no Text
            display_gifs_update(&mctx, &ga, &gb, -1);
            break;
        }

        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);

        frames_in_mode++;
        if (frames_in_mode >= mode_frames)
        {
            mode = (mode + 1) % 3;
            frames_in_mode = 0;
            mode_frames = rand_range(mode_min_frames, mode_max_frames);
        }
    }
    led_matrix_delete(mctx.matrix);
    free_gif_frames(&ga);
    free_gif_frames(&gb);
    delete_font(text.font);
    free(text.text);
    free(bottom_text.text);

    return 0;
}