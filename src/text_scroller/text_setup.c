#include "../../main.h"

static void init_text(Text *text, struct LedFont *font, int font_width, 
                     int font_baseline, struct Color color, int x_orig, 
                     int y_orig, TextLines *text_lines)
{
    text->font = font;
    text->font_width = font_width;
    text->font_baseline = font_baseline;
    text->color = color;
    text->x_orig = x_orig;
    text->y_orig = y_orig;
    text->x = x_orig;
    text->y = y_orig;
    text->lines = text_lines;
    text->letter_spacing = 0;
}

int text_setup(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || top == NULL || bottom == NULL)
        return 0;

    struct LedFont *font = load_font("fonts/unifont.bdf");
    if (font == NULL)
    {
        printf("Couldn't load font 'fonts/unifont.bdf'\n");
        return 0;
    }

    TextLines *text_lines = load_text_lines_from_file("src/text_scroller/ascii.txt");
    if (text_lines == NULL)
    {
        printf("Failed to load text from file.\n");
        delete_font(font);
        return 0;
    }

    // char *text_content = load_text_from_file("src/text_scroller/ascii.txt");
    // if (text_content == NULL)
    // {
    //     printf("Failed to load text from file.\n");
    //     delete_font(font);
    //     return 0;
    // }

    int font_width = character_width_font(font, 'W');
    int font_baseline = baseline_font(font);

    init_text(top, font, font_width, font_baseline, 
              (struct Color){255, 0, 0}, mctx->width + 5, 
              font_baseline, text_lines);
    
    init_text(bottom, font, font_width, font_baseline, 
              (struct Color){0, 0, 255}, mctx->width + 6, 
              font_baseline + 3, text_lines);

    return 1;
}