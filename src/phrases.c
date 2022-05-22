#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

int get_file_lines(int buff_counter, char *buffer)
{
    int l_counter = 0;
    for (int i = 0; i <= buff_counter && i < BUFFER_SIZE; i++)
        if (buffer[i] == '.' || buffer[i] == '!' || buffer[i] == '?' || buffer[i] == '\0')
            l_counter++;
    return l_counter;
}

int main(int argc, char *argv[])
{
    char buffer[BUFFER_SIZE];
    FILE *stream;
    int buff_counter;
    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "usage: phrases [-l] file\n");
        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        stream = fopen(argv[1], "r");
        buff_counter = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);
        printf("%d\n", get_file_lines(buff_counter, &buffer));
    }
    else
    {
        char ch;
        int i = 0, line_num = 1;
        stream = fopen(argv[2], "r");
        while ((ch = fgetc(stream)) != EOF)
        {
            if (ch == '\n')
                continue;
            buffer[i] = ch;
            i++;
            if (ch == '.' || ch == '!' || ch == '?')
            {
                buffer[i] = '\0';
                printf("[%d] %s\n", line_num, buffer);
                line_num++;
                i = 0;
            }
        }
        printf("[%d] %s\n", line_num, buffer);
        fclose(stream);
    }

    return EXIT_SUCCESS;
}
