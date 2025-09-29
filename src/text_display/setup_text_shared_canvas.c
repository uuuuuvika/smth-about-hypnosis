#include "../../main.h"

int text_setup(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || top == NULL || bottom == NULL)
    return 0;

struct Color color_top = {255, 0, 0};
struct Color color_bottom = {0, 0, 255};
    const char *font_file = "fonts/unifont.bdf";

    struct LedFont *font = load_font(font_file);
    if (font == NULL)
    {
        printf("Couldn't load font '%s'\n", font_file);
        return 0;
    }

    int font_width = character_width_font(font, 'W');
    int font_baseline = baseline_font(font);
    printf("Font '%s' loaded: char width=%d, baseline=%d\n", font_file, font_width, font_baseline);

    top->font = font;
    top->font_width = font_width;
    top->font_baseline = font_baseline;
    top->color = color_top;
    top->x_orig = (mctx->width) + 5;
    top->y_orig = font_baseline;
    top->x = top->x_orig;
    top->y = top->y_orig;
    top->letter_spacing = 1;

    bottom->font = font;
    bottom->font_width = font_width;
    bottom->font_baseline = font_baseline;
    bottom->color = color_bottom;
    bottom->x_orig = (mctx->width) + 5;
    bottom->y_orig = font_baseline + 6;
    bottom->x = bottom->x_orig;
    bottom->y = bottom->y_orig;
    bottom->letter_spacing = 1;

    // Load text contents for top and bottom from file
    char *top_text_content = load_text_from_file("src/text_display/ascii.txt");
    if (top_text_content == NULL)
    {
        printf("Failed to load text from file.\n");
        return 0;
    }

    char *bottom_text_content = load_text_from_file("src/text_display/ascii.txt");
    if (bottom_text_content == NULL)
    {
        printf("Failed to load bottom text from file.\n");
        free(top_text_content);
        return 0;
    }

    top->text = top_text_content;
    bottom->text = bottom_text_content;

    return 1;
}

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

    top->x -= 1;
    bottom->x -= 1;
            
    if (top->x + top_text_advance < 0)
        top->x = top->x_orig;
    if (bottom->x + bottom_text_advance < 0)
        bottom->x = bottom->x_orig;    
}