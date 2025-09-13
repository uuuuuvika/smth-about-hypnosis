#include "led-matrix-c.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  struct RGBLedMatrixOptions options;
  struct RGBLedMatrix *matrix;
  struct LedCanvas *offscreen_canvas;
  int width, height;
  int x, y, i;

  memset(&options, 0, sizeof(options));
  options.rows = 32;
  options.cols = 64;
  options.chain_length = 1;
  options.hardware_mapping = "adafruit-hat";
  options.disable_hardware_pulsing = true;
  options.brightness = 50;

  matrix = led_matrix_create_from_options(&options, &argc, &argv);
  if (matrix == NULL)
    return 1;

  offscreen_canvas = led_matrix_create_offscreen_canvas(matrix);
  led_canvas_get_size(offscreen_canvas, &width, &height);

  for (i = 0; i < 100000; ++i) {
    for (y = 0; y < height; ++y) {
      for (x = 0; x < width; ++x) {
        printf("y %d\n", y);
        led_canvas_set_pixel(offscreen_canvas, x, y, 100, 100, y);
      }
    }
    offscreen_canvas = led_matrix_swap_on_vsync(matrix, offscreen_canvas);
  }

  led_matrix_delete(matrix);

  return 0;
}
