#include "../main.h"

int matrix_setup(MatrixContext *ctx)
{
    struct RGBLedMatrixOptions options;
    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;
    options.brightness = 50;

    ctx->matrix = led_matrix_create_from_options(&options, NULL, NULL);
    if (ctx->matrix == NULL)
        return 0;

    ctx->offscreen_canvas = led_matrix_create_offscreen_canvas(ctx->matrix);
    led_canvas_get_size(ctx->offscreen_canvas, &ctx->width, &ctx->height);
    return 1;
}

void overdraw_half(struct LedCanvas *canvas, int width, int height, int left_half)
{
    if (canvas == NULL)
        return;
    int start_x = left_half ? 0 : (width / 2);
    int end_x = left_half ? (width / 2) : width;
    for (int y = 0; y < height; ++y)
    {
        for (int x = start_x; x < end_x; ++x)
        {
            led_canvas_set_pixel(canvas, x, y, 0, 0, 0);
        }
    }
}

int rand_range(int min_inclusive, int max_inclusive)
{
    if (max_inclusive <= min_inclusive)
        return min_inclusive;
    int span = max_inclusive - min_inclusive + 1;
    return min_inclusive + (rand() % span);
}

void show_loading_text(MatrixContext *mctx)
{
    if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL)
        return;
    const char *msg = "I'm loading GIFs...";
    const char *font_file = "fonts/unifont.bdf";
    struct LedFont *font = load_font(font_file);
    if (font == NULL)
    {
        led_canvas_fill(mctx->offscreen_canvas, 0, 0, 0);
        mctx->offscreen_canvas = led_matrix_swap_on_vsync(mctx->matrix, mctx->offscreen_canvas);
        return;
    }

    int font_width = character_width_font(font, 'W');
    int baseline = baseline_font(font);
    int text_width = (int)strlen(msg) * font_width;
    int x = (mctx->width - text_width) / 2;
    if (x < 0)
        x = 0;
    int y = baseline + (mctx->height - baseline) / 2;

    led_canvas_fill(mctx->offscreen_canvas, 0, 0, 0);
    draw_text(mctx->offscreen_canvas, font, x, y, 255, 255, 255, msg, 1);
    mctx->offscreen_canvas = led_matrix_swap_on_vsync(mctx->matrix, mctx->offscreen_canvas);
    delete_font(font);
}
