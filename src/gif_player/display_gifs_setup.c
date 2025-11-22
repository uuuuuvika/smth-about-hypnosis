#include "../../main.h"

// int display_gifs_setup(MatrixContext *mctx, GifContext *a, GifContext *b)
// {
//     if (mctx == NULL || mctx->matrix == NULL || mctx->offscreen_canvas == NULL)
//     {
//         printf("MatrixContext not initialized.\n");
//         return 0;
//     }

//     MagickWandGenesis();

//     a->playlist = NULL;
//     a->playlist_count = 0;
//     b->playlist = NULL;
//     b->playlist_count = 0;
//     a->black_threshold = 0;
//     b->black_threshold = 32;

//     // Show loading notice while preloading from disk
//     show_loading_text(mctx);

//     // Preload all GIFs once to avoid runtime load glitches
//     if (!preload_all_gifs())
//     {
//         printf("Failed to preload GIFs.\n");
//         return 0;
//     }

//     // Pick initial random gifs for both layers from preloaded set
//     if (!load_random_gif_for_layer(a))
//         return 0;
//     if (!load_random_gif_for_layer(b))
//         return 0;

//     return 1;
// }

int display_gifs_setup(MatrixContext *mctx, GifContext *ga, GifContext *gb)
{
    // For ga - use wobbly circle
    create_wobbly_circle_gif(&ga->frames, &ga->frame_count);
    ga->current_frame = 0;
    

    // For gb - use another animation (e.g., bouncing ball)
    create_wobbly_circle_gif(&gb->frames, &gb->frame_count);
    gb->current_frame = 0;
    
    return 1;
}