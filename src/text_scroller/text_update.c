#include "../../main.h"

// void text_update(MatrixContext *mctx, Text *top, Text *bottom)
// {
//     if (mctx == NULL || mctx->offscreen_canvas == NULL || top == NULL || bottom == NULL)
//         return;

//     int top_text_advance = draw_text(mctx->offscreen_canvas, top->font, top->x, top->y,
//                                      top->color.r, top->color.g, top->color.b,
//                                      top->text, top->letter_spacing);

//     int bottom_text_advance = draw_text(mctx->offscreen_canvas, bottom->font, bottom->x, bottom->y,
//                                         bottom->color.r, bottom->color.g, bottom->color.b,
//                                         bottom->text, bottom->letter_spacing);

//     if (--top->x + top_text_advance < 0)
//         top->x = top->x_orig;
//     if (--bottom->x + bottom_text_advance < 0)
//         bottom->x = bottom->x_orig;
// }

// void multi_text_update(MatrixContext *mctx, Text **texts, int count)
// {
//     if (mctx == NULL || mctx->offscreen_canvas == NULL || texts == NULL || count <= 0)
//         return;

//     for (int i = 0; i < count; i++)
//     {
//         Text *current_text = texts[i];

//         if (current_text == NULL)
//             continue;

//         int text_advance = draw_text(mctx->offscreen_canvas,
//                                      current_text->font,
//                                      current_text->x,
//                                      current_text->y,
//                                      current_text->color.r,
//                                      current_text->color.g,
//                                      current_text->color.b,
//                                      current_text->text,
//                                      current_text->letter_spacing);

//         current_text->x--;
//         if (current_text->x + text_advance < 0)
//             current_text->x = current_text->x_orig;
//     }
// }

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