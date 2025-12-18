#include "../../main.h"

void text_update_horizontal(MatrixContext *mctx, Text *text)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || text == NULL)
        return;

    int text_advance = draw_text(mctx->offscreen_canvas, text->font,
                                 text->x, text->y,
                                 text->color.r, text->color.g, text->color.b,
                                 text->text, text->letter_spacing);

    if (--text->x + text_advance < 0)
        text->x = text->x_orig;
}

void text_update_vertical(MatrixContext *mctx, Text *text)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || text == NULL)
        return;

    int text_advance = vertical_draw_text(mctx->offscreen_canvas, text->font,
                                          text->x, text->y,
                                          text->color.r, text->color.g, text->color.b,
                                          text->text, text->letter_spacing);

    if (--text->y + text_advance < 0)
        text->y = text->y_orig;
}

void text_update(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || top == NULL || bottom == NULL)
        return;

    if (top->is_vertical)
        text_update_vertical(mctx, top);
    else
        text_update_horizontal(mctx, top);

    if (bottom->is_vertical)
        text_update_vertical(mctx, bottom);
    else
        text_update_horizontal(mctx, bottom);
}
