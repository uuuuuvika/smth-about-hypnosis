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
    {
        return 0;
    }

    ctx->offscreen_canvas = led_matrix_create_offscreen_canvas(ctx->matrix);
    led_canvas_get_size(ctx->offscreen_canvas, &ctx->width, &ctx->height);
    return 1;
}