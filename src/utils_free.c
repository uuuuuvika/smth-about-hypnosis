#include "../main.h"

void free_gif_frames(GifContext *ctx)
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