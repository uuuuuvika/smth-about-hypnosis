#include "../main.h"

char *load_text_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    int read_size = fread(buffer, 1, length, file);
    fclose(file);

    buffer[read_size] = '\0';

    for (int i = 0; i < read_size; i++)
    {
        unsigned char c = buffer[i];
        if (c < 32 || c == 127)
        {
            //printf("Replacing non-printable character 0x%02x with space\n", c);
            buffer[i] = ' ';
        }
    }

    printf("Loaded %d bytes of UTF-8 text\n", read_size);
    return buffer;
}
