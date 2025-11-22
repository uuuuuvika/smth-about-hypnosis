#include "../main.h"

void free_all_gif_frames(GifContext *ctx)
{
    if (ctx == NULL || ctx->frames == NULL)
        return;

    for (int i = 0; i < ctx->frame_count; ++i)
    {
        if (ctx->frames[i].pixel_data)
        {
            free(ctx->frames[i].pixel_data);
            ctx->frames[i].pixel_data = NULL;
        }
    }
    free(ctx->frames);
    ctx->frames = NULL;
    ctx->frame_count = 0;
}

void free_gif_frames(GifFrame *frames, int frame_count)
{
    for (int i = 0; i < frame_count; i++) {
        free(frames[i].pixel_data);
    }
    free(frames);
}