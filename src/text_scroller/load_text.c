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

// use it:
//  for (size_t i = 0; i < lines->count; i++)
//      printf("Line %zu: %s\n", i, lines->lines[i]);
//  free_text_lines(lines);
TextLines *load_text_lines_from_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = malloc(length + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, length, file);
    buffer[read_size] = '\0';
    fclose(file);

    size_t line_count = 0;
    for (size_t i = 0; i < read_size; i++)
    {
        if (buffer[i] == '\n')
            line_count++;
    }
    if (read_size > 0 && buffer[read_size - 1] != '\n')
        line_count++;

    TextLines *text_lines = malloc(sizeof(TextLines));
    if (!text_lines)
    {
        free(buffer);
        return NULL;
    }

    text_lines->lines = malloc(sizeof(char *) * line_count);
    if (!text_lines->lines)
    {
        free(buffer);
        free(text_lines);
        return NULL;
    }

    text_lines->buffer = buffer;
    text_lines->count = line_count;

    size_t idx = 0;
    char *token = strtok(buffer, "\n\r");
    while (token != NULL)
    {
        text_lines->lines[idx++] = token;
        token = strtok(NULL, "\n\r");
    }

    printf("Loaded %zu lines from file\n", line_count);

    return text_lines;
}