#include "main.h"

int main(int argc, char **argv)
{
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
        free_all_gif_frames(&ga);
        free_all_gif_frames(&gb);
        return 1;
    }

    int mode = 0;
    int mode_min_frames = 180;
    int mode_max_frames = 1600;
    int frames_in_mode = 0;
    int mode_frames = rand_range(mode_min_frames, mode_max_frames);

    int half_width = mctx.width / 2;

    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);

        switch (mode)
        {
        case 0: // Left GIF, Right Text
            text_update(&mctx, &text, &bottom_text);
            //overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1);
            display_gif_on_half(&mctx, &ga, 0, 0, half_width);
            break;
            
        case 1: // Left Text, Right GIF
            display_gif_on_half(&mctx, &gb, half_width, 0, mctx.width - half_width);
            text_update(&mctx, &text, &bottom_text);
            //overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 0);
            break;
            
        case 2: // GIF on both sides
            display_gif_on_half(&mctx, &ga, 0, 0, half_width);
            display_gif_on_half(&mctx, &gb, half_width, 0, mctx.width - half_width);
            usleep(25000);
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