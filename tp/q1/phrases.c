#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 8192
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
    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "usage: phrases [-l] file\n");
        return EXIT_FAILURE;
    }
    char buffer[BUFFER_SIZE];
    int buff_counter, file = argc-1;
    FILE *stream = fopen(argv[file], "r");
    if(stream == NULL) {
        fprintf(stderr, "error: cannot open %s\n", argv[file]);
        return EXIT_FAILURE;
    }

    if (argc == 2)
    {
        buff_counter = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);
        printf("%d\n", get_file_lines(buff_counter, buffer));
    }
    else
    {
        if (strcmp(argv[1], "-l") != 0)
        {
            fprintf(stderr, "usage: phrases [-l] file\n");
            fclose(stream);
            return EXIT_FAILURE;
        }
        char ch;
        int i = 0, line_num = 1;
        while ((ch = fgetc(stream)) != EOF)
        {
            if(i == 0 && ch == ' ')
                continue;
            if (ch == '\n'){
                if(i == 0) continue;
                buffer[i] = ' ';
                i++;
                continue;
            }
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
        if(i != 0) {
            buffer[i] = '\0';
            printf("[%d] %s\n", line_num, buffer);
        }
    }
    fclose(stream);
    return EXIT_SUCCESS;
}
