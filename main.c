#include "main.h"

int main(int argc, char **argv){
    MatrixContext mctx = {0};
    GifContext ga = {0};
    GifContext gb = {0};
    Text top_text = {0};
    Text bottom_text = {0};

    if (!matrix_setup(&mctx))
    {
        printf("Failed to setup matrix.\n");
        return 1;
    }
    if (!display_gifs_setup(&mctx, &ga, &gb))
    {
        printf("Failed to setup dual GIF rendering.\n");
        return 1;
    }
    if (!text_setup(&mctx, &top_text, &bottom_text))
    {
        printf("Failed to setup text rendering.\n");
        return 1;
    }
    
    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);
        display_gifs_update(&mctx, &ga, &gb);
        text_update(&mctx, &top_text, &bottom_text);
        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
        //usleep(100000);
    }

    return 0;
}