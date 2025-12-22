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
    char *text_content = load_text_from_file("src/text_scroller/ascii.txt");
    //char *text_content = "dldksldksldksldksldksldkslkdslkdlskdslkdl dsskdjskldjskhfjskhfskhf sfjskfhskhfskf";
    
    if (text_content == NULL)
    {
        printf("Failed to load text from file.\n");
        delete_font(font);
        return 0;
    }

    top->font = font;
    top->font_width = font_width;
    top->font_baseline = font_baseline;
    top->color = color_top;
    top->x_orig = mctx->width + 5;
    top->y_orig = font_baseline;
    top->x = top->x_orig;
    top->y = top->y_orig;
    top->text = text_content;
    top->letter_spacing = 0;
    top->is_vertical = 0;

    bottom->font = font;
    bottom->font_width = font_width;
    bottom->font_baseline = font_baseline;
    bottom->color = color_bottom;
    bottom->x_orig = mctx->width + 6; 
    bottom->y_orig = font_baseline + 3;
    bottom->x = bottom->x_orig;
    bottom->y = bottom->y_orig;
    bottom->text = text_content;
    bottom->letter_spacing = 0;
    bottom->is_vertical = 0; 

    return 1;
}