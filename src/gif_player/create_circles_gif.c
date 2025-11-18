#include "../../main.h"

void create_circles_gif(GifFrame **frames, int *frame_count)
{
    MagickWand *wand = NewMagickWand();
    DrawingWand *draw = NewDrawingWand();
    PixelWand *bg_color = NewPixelWand();
    PixelWand *circle_color = NewPixelWand();

    PixelSetColor(bg_color, "black");
    PixelSetColor(circle_color, "white");

    MagickNewImage(wand, 128, 32, bg_color);

    DrawSetFillColor(draw, circle_color);
    DrawSetStrokeColor(draw, circle_color);
    DrawSetStrokeWidth(draw, 1);

    DrawCircle(draw, 40, 16, 40 + 12, 16);

    DrawCircle(draw, 55, 16, 55 + 12, 16);

    MagickDrawImage(wand, draw);

    MagickSetImageType(wand, GrayscaleType);

    *frame_count = 1;
    *frames = malloc(sizeof(GifFrame));

    (*frames)[0].width = 128;
    (*frames)[0].height = 32;
    (*frames)[0].delay = 100;
    (*frames)[0].pixel_data = malloc(128 * 32);

    MagickExportImagePixels(wand, 0, 0, 128, 32,
                            "I", CharPixel, (*frames)[0].pixel_data);

    DestroyPixelWand(circle_color);
    DestroyPixelWand(bg_color);
    DestroyDrawingWand(draw);
    DestroyMagickWand(wand);
}