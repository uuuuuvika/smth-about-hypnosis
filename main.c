#include "main.h"

int main(int argc, char **argv){
    MatrixContext mctx = {0};
    GifContext ga = {0};
    GifContext gb = {0};
    Text text = {0};
    Text bottom_text = {0};

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
    const int mode_min_frames = 180;
    const int mode_max_frames = 1600;
    int frames_in_mode = 0;
    int mode_frames = rand_range_int(mode_min_frames, mode_max_frames);

    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);

        switch (mode) {
            case 0: // A: Text & GIF on both sides
                text_update(&mctx, &text, &bottom_text);
                display_gifs_update(&mctx, &ga, &gb, -1); // both halves
                break;
            case 1: // B: Left GIF only, Right Text only
                text_update(&mctx, &text, &bottom_text);
                overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1); // clear left half
                display_gifs_update(&mctx, &ga, &gb, 0); // draw GIF left half
                break;
            case 2: // C: Left Text only, Right GIF only
                text_update(&mctx, &text, &bottom_text);
                overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 0); // clear right half
                display_gifs_update(&mctx, &ga, &gb, 1); // draw GIF right half
                break;
            case 3: // D: GIF on both sides, no Text
                display_gifs_update(&mctx, &ga, &gb, -1); // both halves
                break;
        }

        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);

        frames_in_mode++;
        if (frames_in_mode >= mode_frames) {
            mode = (mode + 1) % 4;
            frames_in_mode = 0;
            mode_frames = rand_range_int(mode_min_frames, mode_max_frames);
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