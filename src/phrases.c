#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024

char buffer[BUFFER_SIZE];

int get_file_lines(int buff_counter)
{
    int l_counter = 0;
    for (int i = 0; i < buff_counter; i++)
        if (buffer[i] == '.' || buffer[i] == '!' || buffer[i] == '?')
            l_counter++;
    return l_counter;
}

int main(int argc, char *argv[])
{
    FILE *stream;
    int buff_counter;

    if (argc == 1)
    {
        fprintf(stderr, "usage: phrases [-l] file\n");
        return EXIT_FAILURE;
    }
    else if (argc == 2)
    {
        stream = fopen(argv[1], "r");
        buff_counter = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);
        printf("%d\n", get_file_lines(buff_counter));
    }
    else
    {
        int i, j = 0;
        stream = fopen(argv[2], "r");
        buff_counter = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);
        printf("--------\n");
        while (buffer[i] != '\0')
        {
            printf("%c", buffer[i]);
            i++;
        }
        printf("--------\n");
    }

    fclose(stream);
    return EXIT_SUCCESS;
}
