
#include "../../main.h"

void display_gif_on_matrix(const char *gif_filename) {
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width, height;
    int x, y;
    
    GifFrame *frames = NULL;
    int frame_count = 0;
    int current_frame = 0;

    MagickWandGenesis();

    if (!load_gif_frames(gif_filename, &frames, &frame_count)) {
        printf("Failed to load GIF :(\n");
        MagickWandTerminus();
        return;
    }

    printf("Cool coooool, loaded %d frames from GIF\n", frame_count);
    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;
    options.brightness = 50;

    matrix = led_matrix_create_from_options(&options, NULL, NULL);

    if (matrix == NULL) {
        for (int i = 0; i < frame_count; i++) {
            free(frames[i].pixel_data);
        }
        free(frames);
        MagickWandTerminus();
        return;
    }

    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_get_size(offscreen_canvas, &width, &height);

    while (1) {
        GifFrame *current = &frames[current_frame];
        
        for (y = 0; y < height; ++y) {
            for (x = 0; x < width; ++x) {
                int pixel_idx = (y * width + x) * 3;
                int red = current->pixel_data[pixel_idx];
                int green = current->pixel_data[pixel_idx + 1];
                int blue = current->pixel_data[pixel_idx + 2];
                
                led_canvas_set_pixel(offscreen_canvas, x, y, red, green, blue);
            }
        }
        
        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        
        usleep(current->delay * 1000);
        
        current_frame = (current_frame + 1) % frame_count;
    }

    for (int i = 0; i < frame_count; i++) {
        free(frames[i].pixel_data);
    }
    free(frames);
    MagickWandTerminus();
    led_matrix_delete(matrix);
}