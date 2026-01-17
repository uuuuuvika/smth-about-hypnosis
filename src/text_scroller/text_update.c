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

void text_update(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL || top == NULL || bottom == NULL)
        return;

        text_update_horizontal(mctx, top);
        text_update_horizontal(mctx, bottom);
}
