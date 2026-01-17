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
