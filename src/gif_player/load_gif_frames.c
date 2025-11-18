#include "../../main.h"

static PreloadedGif s_preloaded[512];

static int s_preloaded_count = 0;
static int s_preloaded_ready = 0;
const int max_loops = 10;
const int min_loops = 5;

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

        MagickSetImageType(coalesced_wand, GrayscaleType);
        
        MagickResizeImage(coalesced_wand, 128, 32, LanczosFilter);
        
        current_frame->width = MagickGetImageWidth(coalesced_wand);
        current_frame->height = MagickGetImageHeight(coalesced_wand);
        current_frame->delay = MagickGetImageDelay(coalesced_wand);
        if (current_frame->delay == 0) {
            current_frame->delay = 60;
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

int preload_all_gifs()
{
    if (s_preloaded_ready)
        return 1;

    s_preloaded_count = 0;
    printf("Preloading GIFs from %s\n", BASE_PATH);
    DIR *dir = opendir(BASE_PATH);
    if (!dir)
    {
        perror("opendir(BASE_PATH)");
        return 0;
    }
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        const char *fname = ent->d_name;
        size_t len = strlen(fname);
        if (len < 5)
            continue; // need at least 'a.gif'
        if (strcasecmp(fname + len - 4, ".gif") != 0)
            continue;
        if (s_preloaded_count >= (int)(sizeof(s_preloaded) / sizeof(s_preloaded[0])))
            break;

        // Guard against excessively long paths and names
        const size_t full_cap = 512;
        size_t base_len = strlen(BASE_PATH);
        if (base_len + len >= full_cap)
        {
            printf("Skipping GIF (path too long): %s%s\n", BASE_PATH, fname);
            continue;
        }
        if (len >= sizeof(s_preloaded[s_preloaded_count].name))
        {
            printf("Skipping GIF (name too long): %s\n", fname);
            continue;
        }

        char full_path[512];
        // Safe, length-checked build of full path
        snprintf(full_path, sizeof(full_path), "%s%s", BASE_PATH, fname);

        GifFrame *frames = NULL;
        int frame_count = 0;
        if (!load_gif_frames(full_path, &frames, &frame_count))
        {
            printf("Error: Could not load %s\n", full_path);
            continue;
        }
        s_preloaded[s_preloaded_count].frames = frames;
        s_preloaded[s_preloaded_count].frame_count = frame_count;
        // Copy filename safely into fixed buffer
        snprintf(s_preloaded[s_preloaded_count].name,
                 sizeof(s_preloaded[s_preloaded_count].name),
                 "%s", fname);
        s_preloaded_count++;
    }
    closedir(dir);
    if (s_preloaded_count == 0)
        return 0;
    s_preloaded_ready = 1;
    return 1;
}

int load_random_gif_for_layer(GifContext *layer)
{
    if (!s_preloaded_ready || s_preloaded_count <= 0)
    {
        return 0;
    }
    int idx = rand_range(0, s_preloaded_count - 1);
    PreloadedGif *sel = &s_preloaded[idx];
    if (sel->frames == NULL || sel->frame_count <= 0)
        return 0;
    // Assign pointers to preloaded frames (do not free on switch)
    layer->frames = sel->frames;
    layer->frame_count = sel->frame_count;
    layer->current_frame = 0;
    layer->current_path = sel->name;
    layer->loops_remaining = rand_range(min_loops, max_loops);
    return 1;
}
