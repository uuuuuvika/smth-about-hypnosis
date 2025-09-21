#include <wand/MagickWand.h>
#include "../rpi-rgb-led-matrix/include/led-matrix-c.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct {
    unsigned char *pixel_data;
    int width;
    int height;
    int delay;
} GifFrame;

int load_gif_frames(const char *filename, GifFrame **frames, int *frame_count) {
    MagickWand *wand = NewMagickWand();
    
    if (MagickReadImage(wand, filename) == MagickFalse) {
        printf("Error: Could not load %s\n", filename);
        DestroyMagickWand(wand);
        return 0;
    }
    
    MagickWand *coalesced_wand = MagickCoalesceImages(wand);

    if (coalesced_wand == NULL) {
        printf("Error: Could not coalesce GIF frames\n");
        DestroyMagickWand(wand);
        return 0;
    }
    
    *frame_count = MagickGetNumberImages(coalesced_wand);
    printf("GIF has %d frames\n", *frame_count);
    
    *frames = malloc(sizeof(GifFrame) * (*frame_count));
    MagickResetIterator(coalesced_wand);
    
    int i = 0;
    while (MagickNextImage(coalesced_wand) != MagickFalse) {
        GifFrame *current_frame = &(*frames)[i];
        
        MagickResizeImage(coalesced_wand, 128, 32, LanczosFilter, 1.0);
        
        current_frame->width = MagickGetImageWidth(coalesced_wand);
        current_frame->height = MagickGetImageHeight(coalesced_wand);
        current_frame->delay = MagickGetImageDelay(coalesced_wand);
        if (current_frame->delay == 0) {
            current_frame->delay = 100;
        }
        
        current_frame->pixel_data = malloc(current_frame->width * current_frame->height * 3);
        
        //MagickRotationalBlurImage(coalesced_wand, 5.0);
        MagickExportImagePixels(coalesced_wand, 0, 0, current_frame->width, current_frame->height, 
                              "RGB", CharPixel, current_frame->pixel_data);
        i++;
    }
    
    DestroyMagickWand(coalesced_wand);
    DestroyMagickWand(wand);
    return 1; 
}

int main(int argc, char **argv)
{
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width, height;
    int x, y;
    
    GifFrame *frames = NULL;
    int frame_count = 0;
    int current_frame = 0;
    
    const char *gif_filename = "gifs/38.gif";

    MagickWandGenesis();

    if (!load_gif_frames(gif_filename, &frames, &frame_count)) {
        printf("Failed to load GIF frames\n");
        MagickWandTerminus();
        return 1;
    }

    printf("Cool, loaded %d frames from GIF\n", frame_count);

    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = true;
    //options.brightness = 0;

    matrix = led_matrix_create_from_options(&options, &argc, &argv);
    if (matrix == NULL) {
        for (int i = 0; i < frame_count; i++) {
            free(frames[i].pixel_data);
        }
        free(frames);
        MagickWandTerminus();
        return 1;
    }

    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_get_size(offscreen_canvas, &width, &height);

    while (1)
    {
        GifFrame *current = &frames[current_frame];
        
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width; ++x)
            {
                int pixel_idx = (y * width + x) * 3;
                int red = current->pixel_data[pixel_idx];
                int green = current->pixel_data[pixel_idx + 1];
                int blue = current->pixel_data[pixel_idx + 2];
                
                led_canvas_set_pixel(offscreen_canvas, x, y, red, green, blue);
            }
        }
        
        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
        
        usleep(current->delay * 10000);
        
        current_frame = (current_frame + 1) % frame_count;
       
    }

    for (int i = 0; i < frame_count; i++) {
        free(frames[i].pixel_data);
    }
    free(frames);
    MagickWandTerminus();
    led_matrix_delete(matrix);

    return 0;
}