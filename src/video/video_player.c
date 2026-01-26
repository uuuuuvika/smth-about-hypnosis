#include "../../main.h"

// Raw RGB video format:
// - No header, just raw RGB24 pixels
// - Each frame is width * height * 3 bytes
// - Frames are stored sequentially
int video_player_init(VideoPlayer *vp, const char *filepath, int video_fps)
{
    memset(vp, 0, sizeof(VideoPlayer));

    vp->file = fopen(filepath, "rb");
    if (!vp->file)
    {
        fprintf(stderr, "Failed to open video file: %s\n", filepath);
        return 0;
    }

    vp->width = 64;
    vp->height = 32;
    vp->frame_size = vp->width * vp->height * 3;

    // Calculate total frames from file size
    fseek(vp->file, 0, SEEK_END);
    long file_size = ftell(vp->file);
    fseek(vp->file, 0, SEEK_SET);

    vp->total_frames = file_size / vp->frame_size;
    if (vp->total_frames == 0)
    {
        fprintf(stderr, "Video file too small or invalid: %s\n", filepath);
        fclose(vp->file);
        vp->file = NULL;
        return 0;
    }

    // Allocate frame buffer
    vp->frame_buffer = (uint8_t *)malloc(vp->frame_size);
    if (!vp->frame_buffer)
    {
        fprintf(stderr, "Failed to allocate frame buffer\n");
        fclose(vp->file);
        vp->file = NULL;
        return 0;
    }

    // Calculate frame skip for timing
    // e.g., if video is 15fps and target is 60fps, skip = 60/15 = 4
    // vp->frame_skip = target_fps / video_fps;
    // if (vp->frame_skip < 1)
    //     vp->frame_skip = 1;

    vp->frame_skip = 0;
    vp->current_frame = 0;
    vp->frame_counter = 0;
    vp->loop = 1;
    vp->playing = 1;

    printf("Video loaded: %s (%d frames, %dx%d, skip=%d)\n",
           filepath, vp->total_frames, vp->width, vp->height, vp->frame_skip);

    // Read first frame
    fread(vp->frame_buffer, 1, vp->frame_size, vp->file);

    return 1;
}

void video_player_draw(VideoPlayer *vp, MatrixContext *mctx, int x_offset)
{
    if (!vp->playing || !vp->file || !vp->frame_buffer)
        return;

    // Draw current frame to canvas
    uint8_t *pixel = vp->frame_buffer;
    for (int y = 0; y < vp->height && y < mctx->height; y++)
    {
        for (int x = 0; x < vp->width; x++)
        {
            int screen_x = x + x_offset;
            if (screen_x >= 0 && screen_x < mctx->width)
            {
                led_canvas_set_pixel(mctx->offscreen_canvas,
                                     screen_x, y,
                                     pixel[0], pixel[1], pixel[2]);
            }
            pixel += 3;
        }
    }

    // Advance frame based on frame skip
    vp->frame_counter++;
    if (vp->frame_counter >= vp->frame_skip)
    {
        vp->frame_counter = 0;
        vp->current_frame++;

        if (vp->current_frame >= vp->total_frames)
        {
            if (vp->loop)
            {
                video_player_reset(vp);
            }
            else
            {
                vp->playing = 0;
                return;
            }
        }

        // Read next frame
        fread(vp->frame_buffer, 1, vp->frame_size, vp->file);
    }
}

void video_player_reset(VideoPlayer *vp)
{
    if (!vp->file)
        return;

    fseek(vp->file, 0, SEEK_SET);
    vp->current_frame = 0;
    vp->frame_counter = 0;
    vp->playing = 1;

    // Read first frame
    fread(vp->frame_buffer, 1, vp->frame_size, vp->file);
}

void video_player_cleanup(VideoPlayer *vp)
{
    if (vp->file)
    {
        fclose(vp->file);
        vp->file = NULL;
    }
    if (vp->frame_buffer)
    {
        free(vp->frame_buffer);
        vp->frame_buffer = NULL;
    }
    vp->playing = 0;
}
