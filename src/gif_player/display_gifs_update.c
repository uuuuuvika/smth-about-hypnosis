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

void draw_frame_to_canvas_scaled(MatrixContext *mctx, GifFrame *frame, int threshold, int x_start, int x_end)
{
    if (x_start < 0)
        x_start = 0;
    if (x_end > mctx->width)
        x_end = mctx->width;
    if (x_start >= x_end)
        return;

    int dest_width = x_end - x_start;
    int dest_height = mctx->height;

    for (int dy = 0; dy < dest_height; ++dy)
    {
        for (int dx = 0; dx < dest_width; ++dx)
        {
            int sx = (dx * frame->width) / dest_width;
            int sy = (dy * frame->height) / dest_height;

            if (sx >= frame->width)
                sx = frame->width - 1;
            if (sy >= frame->height)
                sy = frame->height - 1;

            int idx = (sy * frame->width + sx) * 3;
            int r = frame->pixel_data[idx];
            int g = frame->pixel_data[idx + 1];
            int b = frame->pixel_data[idx + 2];

            led_canvas_set_pixel(mctx->offscreen_canvas, x_start + dx, dy, r, g, b);
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
                layer->loops_remaining = rand_range(10, 20);
            }
        }
    }
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

    // int x_start = 0;
    // int x_end = mctx->width;
    if (half_mode == 0)
        draw_frame_to_canvas_scaled(mctx, fa, a->black_threshold, 0, mid);
    else if (half_mode == 1)
        draw_frame_to_canvas_scaled(mctx, fb, b->black_threshold, mid, mctx->width);
    else
    {
        draw_frame_to_canvas_scaled(mctx, fa, a->black_threshold, 0, mid);
        draw_frame_to_canvas_scaled(mctx, fb, b->black_threshold, mid, mctx->width);
    }

    advance_layer(a);
    advance_layer(b);
}

void display_gif_on_half(MatrixContext *mctx, GifContext *gif, int x_start, int y_start, int width)
{
    if (mctx == NULL || gif == NULL)
        return;
    if (mctx->matrix == NULL || mctx->offscreen_canvas == NULL)
        return;
    if (gif->frames == NULL || gif->frame_count == 0)
        return;

    GifFrame *frame = &gif->frames[gif->current_frame];
    
    int x_end = x_start + width;
    draw_frame_to_canvas_scaled(mctx, frame, gif->black_threshold, x_start, x_end);
    advance_layer(gif);
}