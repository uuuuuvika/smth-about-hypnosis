#include <wand/MagickWand.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    struct RGBLedMatrixOptions options;
    struct RGBLedMatrix *matrix;
    struct LedCanvas *offscreen_canvas;
    int width, height;
    int x, y;

    MagickWandGenesis();
    MagickWand *wand = NewMagickWand();

    if (MagickReadImage(wand, "e.jpg") == MagickFalse) {
        printf("Error: Could not load e.jpg\n");
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return 1;
    }

    printf("Original image: %ux%lu\n", 
           MagickGetImageWidth(wand), MagickGetImageHeight(wand));
      
    if (MagickResizeImage(wand, 128, 32, LanczosFilter, 1.0) == MagickFalse) {
        printf("Error: Could not resize image\n");
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return 1;
    }
    
    size_t image_width = MagickGetImageWidth(wand);
    size_t image_height = MagickGetImageHeight(wand);

    unsigned char *pixel_data = malloc(image_width * image_height * 3);
     if (MagickExportImagePixels(wand, 0, 0, image_width, image_height, "RGB", CharPixel, pixel_data) == MagickFalse) {
        printf("Error: Could not export pixel data\n");
        free(pixel_data);
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return 1;
    }

    printf("Resized to: %ux%u\n", image_width, image_height);

    memset(&options, 0, sizeof(options));
    options.rows = 32;
    options.cols = 64;
    options.chain_length = 2;
    options.hardware_mapping = "adafruit-hat";
    options.disable_hardware_pulsing = false;
    options.brightness = 50;

    matrix = led_matrix_create_from_options(&options, &argc, &argv);
    if (matrix == NULL) {
        free(pixel_data);
        DestroyMagickWand(wand);
        MagickWandTerminus();
        return 1;
    }

    offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
    led_canvas_get_size(offscreen_canvas, &width, &height);

    while (1)
    {
        for (y = 0; y < height; ++y)
        {
            for (x = 0; x < width; ++x)
            {
                int pixel_idx = (y * width + x) * 3;
                int red = pixel_data[pixel_idx];
                int green = pixel_data[pixel_idx + 1];
                int blue = pixel_data[pixel_idx + 2];
                
                led_canvas_set_pixel(offscreen_canvas, x, y, red, green, blue);
            }
        }
        
        offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
    }

    free(pixel_data);
    DestroyMagickWand(wand);
    MagickWandTerminus();
    led_matrix_delete(matrix);

    return 0;
}