#include "../../main.h"

void display_text()
{
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    struct Color color;
    color.r = 255;
    color.g = 255;
    color.b = 0;

    int width, height;

    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;
    options.show_refresh_rate=true;
    // options.brightness = 50;

    const char *font_file = "fonts/unifont.bdf";
    struct LedFont *font = load_font(font_file);
    if (font == NULL)
    {
        printf("Couldn't load font '%s'\n", font_file);
        return;
    }

    int font_width = character_width_font(font, 'W');
    int font_baseline = baseline_font(font);

    char *text = load_text_from_file("test_text.txt");
    if (text == NULL)
    {
        printf("Failed to load text from file.\n");
        free(text);
        return;
    }

    const int x_default_start = (options.chain_length * options.cols) + 5;
    int x_orig = x_default_start;
    int y_orig = 0;
    int letter_spacing = 1;
    float speed = 50.0f;
    int loops = -1;

    matrix = led_matrix_create_from_options(&options, NULL, NULL);
    if (matrix == NULL)
    {
        printf("Failed to create matrix\n");
        delete_font(font);
        return;
    }
    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_get_size(offscreen_canvas, &width, &height);

    int delay_speed_usec = 10000;
    if (speed > 0)
        delay_speed_usec = 10000 / speed / font_width;
    else if (x_orig == x_default_start)
        x_orig = 0;

    int x = x_orig;
    int y = y_orig;
    int length = 0;

    while (loops != 0)
    {
        led_canvas_fill(offscreen_canvas, 0, 0, 0);
        length = draw_text(offscreen_canvas, font, x, y + font_baseline * 2,
                           color.r , color.g, color.b,
                           text, letter_spacing);

        if (speed > 0 && (x -= font_width) + length < 0)
        {
            x = x_orig;
            if (loops > 0)
                --loops;
        }

        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        //usleep(delay_speed_usec);
    }

    led_matrix_delete(matrix);
    delete_font(font);

    free(text);
}