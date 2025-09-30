#include "../../main.h"

void text_update(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || mctx->offscreen_canvas == NULL) return;
    if (top == NULL || bottom == NULL) return;

    int top_text_advance = draw_text(mctx->offscreen_canvas, top->font, top->x, top->y,
        top->color.r, top->color.g, top->color.b,
        top->text, top->letter_spacing);
            
    int bottom_text_advance = draw_text(mctx->offscreen_canvas, bottom->font, bottom->x, bottom->y,
        bottom->color.r, bottom->color.g, bottom->color.b,
        bottom->text, bottom->letter_spacing);

    if (--top->x + top_text_advance < 0)
        top->x = top->x_orig;
    if (--bottom->x + bottom_text_advance < 0)
        bottom->x = bottom->x_orig;    
}