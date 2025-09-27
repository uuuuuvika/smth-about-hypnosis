#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "gif") == 0)
        {
            MatrixContext mctx = {0};
            GifContext ga = {0};
            GifContext gb = {0};
            if (!matrix_setup(&mctx))
            {
                printf("Failed to setup matrix.\n");
                return 1;
            }
            if (!display_gifs_playlist_setup(&mctx, &ga, &gb))
            {
                printf("Failed to setup playlist GIF rendering.\n");
                return 1;
            }
            while (1)
            {
                led_canvas_fill(mctx.offscreen_canvas, 0, 0, 255);
                display_gifs_playlist_update(&mctx, &ga, &gb);
                mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
                usleep(100000);
            }
        }
        else if (strcmp(argv[1], "text") == 0)
        {
            display_text();
        }
        else if (strcmp(argv[1], "pro") == 0)
        {
            MatrixContext mctx = {0};
            GifContext ga = {0};
            GifContext gb = {0};

            Text top_text = {0};
            Text bottom_text = {0};

            char *top_text_content = load_text_from_file("src/text_display/ascii.txt");
            if (top_text_content == NULL)
            {
                printf("Failed to load text from file.\n");
                free(top_text_content);
                return 1;
            }
            char *bottom_text_content = load_text_from_file("src/text_display/ascii.txt");
            if (bottom_text_content == NULL)
            {
                printf("Failed to load bottom text from file.\n");
                free(top_text_content);
                free(bottom_text_content);
                return 1;
            }
            top_text.text = top_text_content;
            bottom_text.text = bottom_text_content;
            if (!matrix_setup(&mctx))
            {
                printf("Failed to setup matrix.\n");
                return 1;
            }
            if (!display_two_gifs_setup(&mctx, &ga, &gb, "gifs/2.gif", "gifs/3.gif"))
            {
                printf("Failed to setup dual GIF rendering.\n");
                return 1;
            }
            if (!setup_text_shared_canvas(&mctx, &top_text, &bottom_text))
            {
                printf("Failed to setup text rendering.\n");
                return 1;
            }
            int frame_count = 0;
            while (1)
            {
                led_canvas_fill(mctx.offscreen_canvas, 0, 0, 0);
                display_two_gifs_update(&mctx, &ga, &gb);
                int top_text_advance = draw_text(mctx.offscreen_canvas, top_text.font, top_text.x, top_text.y,
                                             top_text.color.r, top_text.color.g, top_text.color.b,
                                             top_text.text, top_text.letter_spacing);
                
                int bottom_text_advance = draw_text(mctx.offscreen_canvas, bottom_text.font, bottom_text.x, bottom_text.y,
                                             bottom_text.color.r, bottom_text.color.g, bottom_text.color.b,
                                             bottom_text.text, bottom_text.letter_spacing);
                top_text.x -= 1;
                bottom_text.x -= 1;

                if (top_text.x + top_text_advance < 0)
                    top_text.x = top_text.x_orig;
                if (bottom_text.x + bottom_text_advance < 0)
                    bottom_text.x = bottom_text.x_orig;    
 
                    
                mctx.offscreen_canvas = led_matrix_swap_on_vsync(mctx.matrix, mctx.offscreen_canvas);
                //usleep(100000);
                frame_count++;
            }
        }
        else
        {
            printf("Usage: %s [gif <filename> | text | implement <something>, Esteban!]\n", argv[0]);
        }
    }
    else
    {
        printf("Usage: %s [gif <A.gif> <B.gif> | gifp <gif...> | text]\n", argv[0]);
    }

    return 0;
}