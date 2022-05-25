#include <sys/mman.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#define READ_END 0
#define WRITE_END 1
#define LINE_SIZE 256
#define BUFFER_SIZE 8192
#define KEYWORDS 2

char* replaceWord(const char* s, const char keywords[LINE_SIZE][KEYWORDS][BUFFER_SIZE], int keywords_size)
{
    char* result;
    int i, k, str_size = 0;
    result = (char*)malloc(1);
  
    // Counting the number of times old words occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        for(k = 0; k < LINE_SIZE && k < keywords_size; k++) {
            if (strstr(&s[i], keywords[k][0]) == &s[i]) {
                int size = strlen(keywords[k][0]) + (strlen(keywords[k][1]) - strlen(keywords[k][0]));
                str_size+= size;
                result = (char*) realloc(result, i+str_size+1);
                strcpy(&result[str_size-size], keywords[k][1]);
                i += strlen(keywords[k][0]);
                break;
            }
            else if (strstr(&s[i], keywords[k][1]) == &s[i]) {
                int size = strlen(keywords[k][1]) + (strlen(keywords[k][0]) - strlen(keywords[k][1]));
                str_size+= size;
                result = (char*) realloc(result, i+str_size+1);
                strcpy(&result[str_size-size], keywords[k][0]);
                i += strlen(keywords[k][1]);
                break;
            }
        }
        result = (char*) realloc(result, i+str_size+2);
        result[str_size] =s[i];
        str_size++;
    }
    result[i] = '\0';
    return result;
}

int main(int argc, char* argv[]) {
    int nbytes, fd[2], fd2[2], i = 0;
    pid_t pid;
    char child_buffer[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char c;
    FILE *cypher;
    while((c = getc(stdin)) != EOF) {
        buffer[i] = c;
        i++;
    }
    if (pipe(fd) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if (pipe(fd2) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        close(fd[READ_END]);
        close(fd2[WRITE_END]);
        /* parent */

        if ((nbytes = write(fd[WRITE_END], buffer, i)) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }
        close(fd[WRITE_END]);

        /* wait for child and exit */

        if ( waitpid(pid, NULL, 0) < 0) {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
        }

        if ((nbytes = read(fd2[READ_END], child_buffer, BUFFER_SIZE)) < 0 ) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd2[READ_END]);

        printf("%s", child_buffer);

        exit(EXIT_SUCCESS);
    }
    else {
        /* child */
        close(fd[WRITE_END]);
        close(fd2[READ_END]);

        cypher = fopen("cypher.txt", "r");
        if(!cypher) {
            fprintf(stderr, "Cannot open 'cypher.txt' for child: %s\n", strerror(errno));
            exit(EXIT_FAILURE);
        }

        char keywords[LINE_SIZE][KEYWORDS][BUFFER_SIZE];
        char aux[BUFFER_SIZE];
        char *token;
        int i = 0;
        while(fgets(aux, LINE_SIZE, cypher)) {
            token = strtok(aux, " ");
            if(token == NULL) {
                printf("token NULL\n");
                exit(EXIT_FAILURE);
            }
            memcpy(keywords[i][0], token, strlen(token)+1);
            token = strtok(NULL, "\n");
            if(token == NULL) {
                printf("token NULL\n");
                exit(EXIT_FAILURE);
            }
            memcpy(keywords[i][1], token, strlen(token)+1);            
            i++;
        }

        int keywords_size = i;
        if ((nbytes = read(fd[READ_END], child_buffer, BUFFER_SIZE)) < 0 ) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd[READ_END]);

        char * str = replaceWord(child_buffer, keywords, keywords_size);
        //printf("%s", str);

        /* write message to parent */
        if ((nbytes = write(fd2[WRITE_END], str, BUFFER_SIZE)) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }
        close(fd2[WRITE_END]);

        /* exit gracefully */
        fclose(cypher);
        exit(EXIT_SUCCESS);
    }
}
