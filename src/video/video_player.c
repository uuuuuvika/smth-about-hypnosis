#include "../../main.h"

// Raw RGB video format:
// - No header, just raw RGB24 pixels
// - Each frame is width * height * 3 bytes
// - Frames are stored sequentially
int video_player_init(VideoPlayer *vp, const char *filepath, int video_fps, int prjct_fps)
{
    memset(vp, 0, sizeof(VideoPlayer));

    vp->file = fopen(filepath, "rb");
    if (!vp->file)
    {
        fprintf(stderr, "Failed to open video file: %s\n", filepath);
        return 0;
    }

    vp->width = MATRIX_WIDTH / 2;
    vp->height = MATRIX_HEIGHT;
    vp->frame_size = vp->width * vp->height * 3;

    fseek(vp->file, 0, SEEK_END);
    long file_size = ftell(vp->file);
    fseek(vp->file, 0, SEEK_SET);

    vp->total_frames = file_size / vp->frame_size;
    if (vp->total_frames <= 0)
    {
        fclose(vp->file);
        return 0;
    }

    vp->frame_buffer = (uint8_t *)malloc(vp->frame_size);
    if (!vp->frame_buffer)
    {
        fclose(vp->file);
        return 0;
    }

    // How many video frames per 1 project tick?
    vp->frame_step = (float)video_fps / (float)prjct_fps;
    vp->frame_accumulator = 0.0f;

    vp->current_frame = 0;
    vp->loop = 1;
    vp->playing = 1;

    fread(vp->frame_buffer, 1, vp->frame_size, vp->file);
    return 1;
}

void video_player_draw(VideoPlayer *vp, MatrixContext *mctx, int x_offset)
{
    if (!vp->playing || !vp->file || !vp->frame_buffer)
        return;

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

    vp->frame_accumulator += vp->frame_step;
    if (vp->frame_accumulator >= 1.0f)
    {
        int frames_to_advance = (int)vp->frame_accumulator;
        vp->frame_accumulator -= (float)frames_to_advance;
        vp->current_frame += frames_to_advance;

        if (vp->current_frame >= vp->total_frames)
        {
            if (vp->loop)
            {
                fseek(vp->file, 0, SEEK_SET);
                vp->current_frame = 0;
            }
            else
            {
                vp->playing = 0;
                return;
            }
        }
        else if (frames_to_advance > 1)
            fseek(vp->file, (frames_to_advance - 1) * vp->frame_size, SEEK_CUR);
        fread(vp->frame_buffer, 1, vp->frame_size, vp->file);
    }
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
