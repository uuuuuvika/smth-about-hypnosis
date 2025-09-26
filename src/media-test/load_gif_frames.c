#include "../../main.h"

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
            current_frame->delay = 10;
        }
        
        current_frame->pixel_data = malloc(current_frame->width * current_frame->height * 3);
        
        MagickExportImagePixels(coalesced_wand, 0, 0, current_frame->width, current_frame->height, 
                              "RGB", CharPixel, current_frame->pixel_data);
        i++;
    }
    
    DestroyMagickWand(coalesced_wand);
    DestroyMagickWand(wand);
    return 1; 
}
