#include "../../main.h"

void text_update_vertical(MatrixContext *mctx, Text *text, int x_offset, int should_move)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || text == NULL)
        return;
    if (text->lines == NULL || text->lines->count == 0)
        return;

    // Draw each visible line
    for (size_t i = 0; i < text->lines->count; i++)
    {
        int line_y = text->y + (i * text->font_height);

        // Only draw if line is potentially visible on screen
        if (line_y > -text->font_height && line_y < mctx->height + text->font_height)
        {
            draw_text(mctx->offscreen_canvas, text->font,
                      x_offset, line_y,
                      text->color.r, text->color.g, text->color.b,
                      text->lines->lines[i], text->letter_spacing);
        }
    }

    if (should_move)
    {
        text->y--;
        // Reset when all text has scrolled off the top
        if (text->y + text->total_height < 0)
        {
            text->y = text->y_orig;
        }
    }
}

void text_update(MatrixContext *mctx, Text *top, Text *bottom, int should_move)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || top == NULL || bottom == NULL)
        return;
    text_update_vertical(mctx, top, 0, should_move);
    // text_update_vertical(mctx, bottom, 1, should_move);
}
