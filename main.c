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
        led_matrix_destroy(mctx.matrix);
        return 1;
    }
    if (!display_gifs_setup(&mctx, &ga, &gb))
    {
        printf("Failed to setup dual GIF rendering.\n");
        led_matrix_destroy(mctx.matrix);
        free_gif_frames(&ga);
        free_gif_frames(&gb);
        delete_font(top_text.font);
        free(top_text.text);
        free(bottom_text.text);
        return 1;
    }
    if (!text_setup(&mctx, &top_text, &bottom_text))
    {
        printf("Failed to setup text rendering.\n");
        led_matrix_destroy(mctx.matrix);
        free_gif_frames(&ga);
        free_gif_frames(&gb);
        delete_font(top_text.font);
        free(top_text.text);
        free(bottom_text.text);
        return 1;
    }
    
    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);
        int top_text_advance = draw_text(mctx.offscreen_canvas, top_text.font, top_text.x, top_text.y,
            top_text.color.r, top_text.color.g, top_text.color.b,
            top_text.text, top_text.letter_spacing);
            
            int bottom_text_advance = draw_text(mctx.offscreen_canvas, bottom_text.font, bottom_text.x, bottom_text.y,
                bottom_text.color.r, bottom_text.color.g, bottom_text.color.b,
                bottom_text.text, bottom_text.letter_spacing);
                top_text.x -= 1;
                bottom_text.x -= 1;
                
                if (top_text.x + top_text_advance < 0)
                top_text.x = top_text.x_orig;
            if (bottom_text.x + bottom_text_advance < 0)
            bottom_text.x = bottom_text.x_orig;    
        
        display_gifs_update(&mctx, &ga, &gb);
            
        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
    }
    led_matrix_destroy(mctx.matrix);
    free_gif_frames(&ga);
    free_gif_frames(&gb);
    delete_font(top_text.font);
    free(top_text.text);
    free(bottom_text.text);

    return 0;
}