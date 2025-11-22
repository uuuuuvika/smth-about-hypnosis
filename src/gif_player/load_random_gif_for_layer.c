#include "../../main.h"


static PreloadedGif s_preloaded[512];
static int s_preloaded_count = 0;
static int s_preloaded_ready = 0;
const int max_loops = 10;
const int min_loops = 5;

int load_random_gif_for_layer(GifContext *layer)
{
    if (!s_preloaded_ready || s_preloaded_count <= 0)
        return 0;

    int idx = rand_range(0, s_preloaded_count - 1);
    PreloadedGif *sel = &s_preloaded[idx];
    if (sel->frames == NULL || sel->frame_count <= 0)
        return 0;

    layer->frames = sel->frames;
    layer->frame_count = sel->frame_count;
    layer->current_frame = 0;
    layer->current_path = sel->name;
    layer->loops_remaining = rand_range(min_loops, max_loops);

    return 1;
}