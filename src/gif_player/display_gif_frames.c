#include "../../main.h"

void draw_frame_to_canvas(MatrixContext *mctx, GifFrame *frame, int threshold, int x_start, int x_end)
{
    if (x_start < 0)
        x_start = 0;
    if (x_end > mctx->width)
        x_end = mctx->width;
    if (x_start >= x_end)
        return;

    for (int y = 0; y < mctx->height; ++y)
    {
        for (int x = x_start; x < x_end; ++x)
        {
            int idx = (y * mctx->width + x) * 3;
            int r = frame->pixel_data[idx];
            int g = frame->pixel_data[idx + 1];
            int b = frame->pixel_data[idx + 2];
            led_canvas_set_pixel(mctx->offscreen_canvas, x, y, r, g, b);
        }
    }
}

void advance_layer(GifContext *layer)
{
    layer->current_frame++;
    if (layer->current_frame >= layer->frame_count)
    {
        layer->current_frame = 0;
        if (layer->loops_remaining > 0)
            layer->loops_remaining--;
        if (layer->loops_remaining == 0)
        {
            if (!load_random_gif_for_layer(layer))
            {
                // best-effort fallback: keep current and reset loops
                layer->loops_remaining = rand_range(10, 20);
            }
        }
    }
    sleep(2000);
}

void display_gifs_update(MatrixContext *mctx, GifContext *a, GifContext *b, int half_mode)
{
    if (mctx == NULL || a == NULL || b == NULL)
        return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL)
        return;
    if (a->frames == NULL || a->frame_count == 0)
        return;
    if (b->frames == NULL || b->frame_count == 0)
        return;

    GifFrame *fa = &a->frames[a->current_frame];
    GifFrame *fb = &b->frames[b->current_frame];
    int mid = mctx->width / 2;

    int x_start = 0;
    int x_end = mctx->width;
    if (half_mode == 0)
    {
        x_start = 0;
        x_end = mid;
    }
    else if (half_mode == 1)
    {
        x_start = mid;
        x_end = mctx->width;
    }

    draw_frame_to_canvas(mctx, fa, a->black_threshold, x_start, x_end);
    draw_frame_to_canvas(mctx, fb, b->black_threshold, x_start, x_end);

    advance_layer(a);
    advance_layer(b);
}