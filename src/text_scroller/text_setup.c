#include "../../main.h"

static void init_text(Text *text, struct LedFont *font, int font_width,
                      int font_height, int font_baseline, struct Color color,
                      int y_orig, TextLines *lines, int screen_height)
{
    text->font = font;
    text->font_width = font_width;
    text->font_height = font_height;
    text->font_baseline = font_baseline;
    text->color = color;
    text->lines = lines;
    text->y_orig = y_orig;
    text->y = y_orig;
    text->letter_spacing = 0;
    // Calculate total height of all text lines
    text->total_height = lines->count * font_height;
}

int text_setup(MatrixContext *mctx, Text *top, Text *bottom)
{
    if (mctx == NULL || top == NULL || bottom == NULL)
        return 0;

    // Use smaller font for vertical scrolling (5x8 pixels)
    struct LedFont *font = load_font("fonts/5x8.bdf");
    if (font == NULL)
    {
        printf("Couldn't load font 'fonts/5x8.bdf'\n");
        return 0;
    }

    TextLines *text_lines = load_text_lines_from_file("src/text_scroller/ascii.txt");
    if (text_lines == NULL)
    {
        printf("Failed to load text from file.\n");
        delete_font(font);
        return 0;
    }

    int font_width = character_width_font(font, 'W');
    int font_height = height_font(font);
    int font_baseline = baseline_font(font);

    // Start text below screen (will scroll up)
    int start_y = mctx->height + font_baseline;

    init_text(top, font, font_width, font_height, font_baseline,
              (struct Color){255, 0, 0}, start_y, text_lines, mctx->height);

    init_text(bottom, font, font_width, font_height, font_baseline,
              (struct Color){0, 0, 255}, start_y + 1, text_lines, mctx->height);

    return 1;
}
