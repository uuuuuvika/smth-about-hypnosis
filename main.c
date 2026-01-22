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
    Text text = {0};
    Text bottom_text = {0};
    FrameController fc = {0};
    VideoPlayer video = {0};
    int video_enabled = 0;

    signal(SIGINT, handle_signal);
    signal(SIGTSTP, handle_signal);

    if (!matrix_setup(&mctx))
    {
        printf("Failed to setup matrix.\n");
        return 1;
    }
    if (!text_setup(&mctx, &text, &bottom_text))
    {
        printf("Failed to setup text rendering.\n");
        led_matrix_delete(mctx.matrix);
        return 1;
    }

    particle_animation_init(&particle_anim);

    if (video_player_init(&video, "assets/output_videos/bw_test.rgb", 60))
    {
        video_enabled = 1;
        printf("Video playback enabled.\n");
    }
    else
    {
        printf("No video file found, video mode disabled.\n");
    }

    init_frame_controller(&fc, 60, 20, 60);

    int mode = 3;
    int frames_in_mode = 0;
    int mode_min_frames = 180;
    int mode_max_frames = 1600;
    int mode_frames = rand_range(mode_min_frames, mode_max_frames);

    int half_width = mctx.width / 2;

    while (running)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);

        switch (mode)
        {
        case 0: // Left GIF, Right Text
            text_update(&mctx, &text, &bottom_text);
            overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 0);
            particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
            break;

        case 1:
            text_update(&mctx, &text, &bottom_text,
                        fc.text_frame_counter % fc.text_frame_skip == 0);
            overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1);
            particle_animation_draw(&particle_anim, &mctx, 0, half_width);
            break;
            
        case 2: // GIF on both sides
            particle_animation_draw(&particle_anim, &mctx, 0, half_width);
            particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
            usleep(25000);
            break;

        case 3:
            // Video on left half, particles on right half
            if (video_enabled)
            {
                text_update(&mctx, &text, &bottom_text,
                            fc.text_frame_counter % fc.text_frame_skip == 0);
                overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, 1);
                video_player_draw(&video, &mctx, 0);

                // video_player_draw(&video, &mctx, half_width);
                // particle_animation_draw(&particle_anim, &mctx, half_width, half_width);
            }
            break;
        }

        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);

        frames_in_mode++;
        // if (frames_in_mode >= mode_frames)
        // {
        //     mode = (mode + 1) % (video_enabled ? 4 : 3);
        //     frames_in_mode = 0;
        //     mode_frames = rand_range(mode_min_frames, mode_max_frames);
        // }
    }
    
    printf("\x1b[36m\nShutting down gracefully.\n\x1b[0m");

    video_player_cleanup(&video);
    led_matrix_delete(mctx.matrix);
    delete_font(text.font);
    free(text.text);

    return 0;
}