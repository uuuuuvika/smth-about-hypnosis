#include "main.h"

volatile sig_atomic_t running = 1;

void handle_signal(int sig)
{
    (void)sig;
    running = 0;
}

int main(int argc, char **argv)
{
    MatrixContext mctx = {0};
    ParticleAnimation particle_anim = {0};
    // GifContext ga = {0};
    // GifContext gb = {0};
    // Text text = {0};
    // Text bottom_text = {0};

    signal(SIGINT, handle_signal);
    signal(SIGTSTP, handle_signal);

    if (!matrix_setup(&mctx))
    {
        printf("Failed to setup matrix.\n");
        return 1;
    }
    // if (!display_gifs_setup(&mctx, &ga, &gb))
    // {
    //     printf("Failed to setup dual GIF rendering.\n");
    //     led_matrix_delete(mctx.matrix);
    //     return 1;
    // }
    // if (!text_setup(&mctx, &text, &bottom_text))
    // {
    //     printf("Failed to setup text rendering.\n");
    //     led_matrix_delete(mctx.matrix);
    //     // free_all_gif_frames(&ga);
    //     // free_all_gif_frames(&gb);
    //     return 1;
    // }

    particle_animation_init(&particle_anim);

    // int mode = 0;
    // int mode_min_frames = 180;
    // int mode_max_frames = 1600;
    // int frames_in_mode = 0;
    // int mode_frames = rand_range(mode_min_frames, mode_max_frames);

    int half_width = mctx.width / 2;

    while (running)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);
        particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
        // switch (mode)
        // {
        // case 0: // Left GIF, Right Text
        //     text_update(&mctx, &text, &bottom_text);
        //     //overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1);
        //     particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
        //     break;
            
        // case 1: // Left Text, Right GIF
        //     particle_animation_draw(&particle_anim, &mctx, 0, half_width);
        //     text_update(&mctx, &text, &bottom_text);
        //     //overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 0);
        //     break;
            
        // case 2: // GIF on both sides
        //     particle_animation_draw(&particle_anim, &mctx, 0, half_width);
        //     particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
        //     usleep(25000);
        //     break;
        // }

        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);

        // frames_in_mode++;
        // if (frames_in_mode >= mode_frames)
        // {
        //     mode = (mode + 1) % 3;
        //     frames_in_mode = 0;
        //     mode_frames = rand_range(mode_min_frames, mode_max_frames);
        // }
    }
    
    printf("\nShutting down gracefully...\n");

    led_matrix_delete(mctx.matrix);
    //delete_font(text.font);
    //free_all_gif_frames(&ga);
    //free_all_gif_frames(&gb);
    //free(text.text);
   // free(bottom_text.text);

    return 0;
}