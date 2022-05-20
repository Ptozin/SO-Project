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

int new_line(char buffer_char) {
    if(buffer_char == '.' || buffer_char == '!' || buffer_char == '?')
        return 1;
    return 0;
}

int main(int argc, char *argv[])
{
    char *buffer;
        buffer = (char*)malloc(sizeof(char)*BUFFER_SIZE);
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
    else {
        stream = fopen(argv[2], "r");
        buff_counter = fread(buffer, sizeof(char), BUFFER_SIZE, stream);
        printf("--------\n");
        int line = 1;
        char *ptr = buffer;
        const char str2[] = "'\n'";
        const char str3[] = "'\0'";
        int len = strcspn(buffer, str2);
        int end = strcspn(buffer, str3);
        while(len < end) {
            buffer[len] = ' ';
            len = strcspn(buffer, str2);
        }
        printf("%s\n", buffer);

        printf("[%d] ", line);
        while (*ptr != '\0') {
            printf("%c", *ptr);
            if(new_line(*ptr)) {
                line++;
                printf("\n[%d]", line);
            }
            ptr++;
        }
        printf("\n--------\n");
        

        fclose(stream);
    }
    return EXIT_SUCCESS;
}
