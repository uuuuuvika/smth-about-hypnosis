#include "../../main.h"

void display_text()
{
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;

    struct Color color_top = {255, 0, 0};
    struct Color color_bottom = {0, 0, 255};

    int width, height;

    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;

    const char *font_file = "fonts/unifont.bdf";
    struct LedFont *font = load_font(font_file);
    if (font == NULL)
    {
        printf("Couldn't load font '%s'\n", font_file);
        return;
    }

    int font_width = character_width_font(font, 'W');
    int font_baseline = baseline_font(font);

    printf("Font '%s' loaded: char width=%d, baseline=%d\n", font_file, font_width, font_baseline);

    char *top_text = load_text_from_file("src/text_display/ascii.txt");
    if (top_text == NULL)
    {
        printf("Failed to load text from file.\n");
        free(top_text);
        return;
    }

    char *bottom_text = top_text;
    // char *bottom_text = load_text_from_file("src/text_display/ascii.txt");
    // if (bottom_text == NULL)
    // {
    //     printf("Failed to load bottom text from file.\n");
    //     free(top_text);
    //     delete_font(font);
    //     return;
    // }

    const int x_default_start = (options.chain_length * options.cols) + 5;
    int top_x_orig = x_default_start;
    int bottom_x_orig = x_default_start;

    int letter_spacing = 1;
    float speed = 5.0f;

    matrix = led_matrix_create_from_options(&options, NULL, NULL);
    if (matrix == NULL)
    {
        printf("Failed to create matrix\n");
        delete_font(font);
        return;
    }
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_get_size(offscreen_canvas, &width, &height);

    printf("Matrix created: %dx%d\n", width, height);

    int delay_speed_usec = 10000;
    if (speed > 0)
        delay_speed_usec = 10000 / speed / font_width;
    else
    {
        if (top_x_orig == x_default_start)
            top_x_orig = 0;
        if (bottom_x_orig == x_default_start)
            bottom_x_orig = 0;
    }

    int top_x = top_x_orig;
    int bottom_x = bottom_x_orig;

    int top_y = font_baseline;
    int bottom_y = font_baseline + (font_baseline / 2);
    //int length = 0;

    while (1)
    {
        led_canvas_fill(offscreen_canvas, 0, 0, 0);

        int top_length = draw_text(offscreen_canvas, font, top_x, top_y,
                                   color_top.r, color_top.g, color_top.b,
                                   top_text, letter_spacing);
        printf("top_x=%d top_len=%d\n", top_x, top_length);

        int bottom_length = draw_text(offscreen_canvas, font, bottom_x, bottom_y,
                                      color_bottom.r, color_bottom.g, color_bottom.b,
                                      bottom_text, letter_spacing);
        printf("bottom_x=%d bottom_len=%d\n", bottom_x, bottom_length);

        if (speed > 0)
        {
            if ((top_x--) + top_length < 0)
                top_x = top_x_orig;
            if ((bottom_x--) + bottom_length < 0)
                bottom_x = bottom_x_orig;
        }


        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        usleep(delay_speed_usec);
    }

    led_matrix_delete(matrix);
    delete_font(font);

    free(top_text);
    free(bottom_text);
}