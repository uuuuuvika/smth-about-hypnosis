#include "../../main.h"

void replace_newlines_with_spaces(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (str[i] == '\n' || str[i] == '\r')
            str[i] = ' ';
    }
}

char *load_text_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);

    size_t read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';
    fclose(file);

    replace_newlines_with_spaces(buffer);

    printf("Loaded %zu bytes of UTF-8 text\n", read_size);
    return buffer;
}
