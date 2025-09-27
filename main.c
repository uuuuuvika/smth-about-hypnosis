#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "gif") == 0)
        {
            MatrixContext mctx = {0};
            GifContext ga = {0};
            GifContext gb = {0};
            if (!matrix_setup(&mctx))
            {
                printf("Failed to setup matrix.\n");
                return 1;
            }
            if (!display_gifs_playlist_setup(&mctx, &ga, &gb))
            {
                printf("Failed to setup playlist GIF rendering.\n");
                return 1;
            }
            while (1)
            {
                led_canvas_fill(mctx.offscreen_canvas, 0, 0, 255);
                display_gifs_playlist_update(&mctx, &ga, &gb);
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
        printf("Usage: %s [gif <A.gif> <B.gif> | gifp <gif...> | text]\n", argv[0]);
    }

    return 0;
}