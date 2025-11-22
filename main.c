#include "main.h"

// Function pointer type for GIF creators
//typedef void (*GifCreator)(GifFrame **frames, int *frame_count);

// static PreloadedGif s_preloaded[512];
// static int s_preloaded_count = 0;
// static int s_preloaded_ready = 0;
// const int max_loops = 10;
// const int min_loops = 5;

// GifCreator get_animation(const char *name)
// {
//     for (int i = 0; animations[i].name != NULL; i++) {
//         if (strcmp(animations[i].name, name) == 0) {
//             return animations[i].creator;
//         }
//     }
//     return NULL;
// }

// void list_animations(void)
// {
//     printf("Available animations:\n");
//     for (int i = 0; animations[i].name != NULL; i++) {
//         printf("  - %s\n", animations[i].name);
//     }
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
        free_all_gif_frames(&ga);
        free_all_gif_frames(&gb);
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
    free_all_gif_frames(&ga);
    free_all_gif_frames(&gb);
    delete_font(text.font);
    free(text.text);
    free(bottom_text.text);

    return 0;
}