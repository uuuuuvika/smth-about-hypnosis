#include "../../main.h"

void text_update_horizontal(MatrixContext *mctx, Text *text, int should_move)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || text == NULL)
        return;
    if (text->lines == NULL || text->lines->count == 0)
        return;

    int text_advance = draw_text(mctx->offscreen_canvas, text->font,
                                 text->x, text->y,
                                 text->color.r, text->color.g, text->color.b,
                                 text->lines->lines[0], text->letter_spacing);

    if (should_move && --text->x + text_advance < 0)
        text->x = text->x_orig;
}

void text_update(MatrixContext *mctx, Text *top, Text *bottom, int should_move)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || top == NULL || bottom == NULL)
        return;

    text_update_horizontal(mctx, top, should_move);
    text_update_horizontal(mctx, bottom, should_move);
}
