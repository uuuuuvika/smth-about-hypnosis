// #include <led-matrix-c.h>
#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 1)
    {
        if (strcmp(argv[1], "gif") == 0 && argc > 2)
        {
            display_gif_on_matrix(argv[2]);
        }
        else if (strcmp(argv[1], "text") == 0)
        {
            display_text();
        }
    }
    else
    {
        printf("Usage: %s [gif <filename> | text | implement <something>, Esteban!]\n", argv[0]);
    }

    return 0;
}