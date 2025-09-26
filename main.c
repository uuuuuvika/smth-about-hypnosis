#include "main.h"

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

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "gif") == 0 && argc > 2)
        {
            MatrixContext mctx = {0};
            GifContext gctx = {0};
            if (!matrix_setup(&mctx))
            {
                printf("Failed to setup matrix.\n");
                return 1;
            }
            if (!display_gif_setup(&mctx, &gctx, argv[2]))
            {
                printf("Failed to setup GIF rendering.\n");
                return 1;
            }
            while (1)
            {
                display_gif_update(&mctx, &gctx);
            }
        }
        else if (strcmp(argv[1], "text") == 0)
        {
            display_text();
        }
    }
    else
    {
        printf("Usage: %s [gif <filename> | text | implement <something>, Esteban!]\n", argv[0]);
    }

    return 0;
}