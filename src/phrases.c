#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFER_SIZE 1024
char buffer[BUFFER_SIZE];

int get_file_lines(int buff_counter)
{
    int l_counter = 0;
    for (int i = 0; i <= buff_counter; i++)
        if (buffer[i] == '.' || buffer[i] == '!' || buffer[i] == '?' || buffer[i] == '\0')
            l_counter++;
    return l_counter;
}

int new_line(char buffer_char) {
    if(buffer_char == '.' || buffer_char == '!' || buffer_char == '?')
        return 1;
    return 0;
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
    stream = fopen(argv[2], "r");
    buff_counter = fread(&buffer, sizeof(char), BUFFER_SIZE, stream);
    printf("--------\n");
    int line = 1;
    char *ptr = buffer;
    printf("[%d] ", line);


    while (*ptr != '\0')
    {
        if(*ptr == '\n')
            *ptr = ' ';
        printf("%c", *ptr);
        if(new_line(*ptr)) {
            line++;
            printf("\n[%d]", line);
        }

        ptr++;

    }
    printf("--------\n");
    

    fclose(stream);
    return EXIT_SUCCESS;
}
