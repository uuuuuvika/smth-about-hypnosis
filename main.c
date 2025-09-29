#include "main.h"

static void overdraw_half(struct LedCanvas *canvas, int width, int height, int left_half)
{
    if (canvas == NULL) return;
    int start_x = left_half ? 0 : (width / 2);
    int end_x = left_half ? (width / 2) : width;
    for (int y = 0; y < height; ++y) {
        for (int x = start_x; x < end_x; ++x) {
            led_canvas_set_pixel(canvas, x, y, 0, 0, 0);
        }
    }
}

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
    
    int frame_counter = 0;
    int overdraw_left = 1;
    const int toggle_every_n_frames = 300;
    while (1)
    {
        led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);
        text_update(&mctx, &top_text, &bottom_text);
        
        if (frame_counter % toggle_every_n_frames == 0) {
            overdraw_left = !overdraw_left;
        }
        overdraw_half(mctx.offscreen_canvas, mctx.width, mctx.height, overdraw_left ? 0 : 1);

        display_gifs_update(&mctx, &ga, &gb, overdraw_left);

        mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
    //usleep(100000);
    frame_counter++;
    }
    led_matrix_destroy(mctx.matrix);
    free_gif_frames(&ga);
    free_gif_frames(&gb);
    delete_font(top_text.font);
    free(top_text.text);
    free(bottom_text.text);

    return 0;
}