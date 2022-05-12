#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int buffer_modifier(char *buffer)
{
    int i = 0;
    int char_num = 0;
    char str_num[10];
    while (1)
    {
        if (strcmp(buffer[char_num], "\n") == 0)
        {
            sprintf(str_num, "[%d] ", i);
            strcat(str_num, buffer);

            i++;
        }
        char_num++;
    }

    return i;
}

int get_buffer_lines(char *buffer)
{
    int lines = 0;
    for (int i = 0; i < strlen(buffer); i++)
        if (!strcmp(buffer[i], '\n'))
            printf("%c\n", buffer[i]);
    return lines;
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "usage: phrases [-l] file\n");
        return EXIT_FAILURE;
    }

    int *line_num = 0;
    char buffer[BUFFER_SIZE];
    char *file = (char *)malloc(sizeof(argv[1]) * sizeof(char));

    strcpy(file, argv[1]);

    FILE *stream;
    stream = fopen(file, "r");
    int count = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);

    if (argc == 2)
    {
        printf("%d\n", get_buffer_lines(buffer));
    }
    fclose(stream);

    return EXIT_SUCCESS;
}
