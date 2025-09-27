#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "gif") == 0 && argc > 3)
        {
            MatrixContext mctx = {0};
            GifContext ga = {0};
            GifContext gb = {0};
            if (!matrix_setup(&mctx))
            {
                printf("Failed to setup matrix.\n");
                return 1;
            }
            if (!display_gifs_setup(&mctx, &ga, &gb, argv[2], argv[3]))
            {
                printf("Failed to setup dual GIF rendering.\n");
                return 1;
            }
            while (1)
            {
                // Reset canvas
                led_canvas_fill(mctx.offscreen_canvas, ga.bg_r, ga.bg_g, ga.bg_b);

                // Updates here
                display_gifs_update(&mctx, &ga, &gb);

                // Write to matrix
                mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
                usleep(100000);
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