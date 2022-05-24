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

char* replaceWord__(const char* s, const char* oldW,
                  const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
  
    // Counting the number of times old word
    // occur in the string
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
  
            // Jumping to index after the old word.
            i += oldWlen - 1;
        }
    }
  
    // Making new string of enough length
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
  
    i = 0;
    while (*s) {
        // compare the substring with the result
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
  
    result[i] = '\0';
    return result;
}

char* replaceword(const char *word, const char keywords[LINE_SIZE][KEYWORDS][BUFFER_SIZE], int keywords_size) {
    char * ret_word;

    for(int i = 0; i < keywords_size; i++) {
        if(word == strstr(word, keywords[i][0])) {
            strcpy(ret_word, keywords[i][1]);
        }
        else if(word == strstr(word, keywords[i][1])) {
            strcpy(ret_word, keywords[i][0]);
        }
    }
    return ret_word;
}


int main(int argc, char* argv[]) {
    int nbytes, fd[2], i = 0;
    pid_t pid;
    char child_buffer[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    char c;
    FILE *cypher;
    while((c = getc(stdin)) != EOF) {
        //printf("%c", c);
        buffer[i] = c;
        i++;
    }
    //printf("%s", buffer);
    printf("\n");

    if (pipe(fd) < 0) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }
    if ((pid = fork()) < 0) {
        perror("fork error");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0) {
        close(fd[READ_END]);
        /* parent */
        printf("Parent process with pid %d\n", getpid());
        printf("Messaging the child process (pid %d):\n", pid);
        //snprintf(aux_buff, LINE_SIZE, "%s", buffer);

        if ((nbytes = write(fd[WRITE_END], buffer, i)) < 0) {
            fprintf(stderr, "Unable to write to pipe: %s\n", strerror(errno));
        }
        close(fd[WRITE_END]);

        /* wait for child and exit */

        if ( waitpid(pid, NULL, 0) < 0) {
            fprintf(stderr, "Cannot wait for child: %s\n", strerror(errno));
        }
        exit(EXIT_SUCCESS);
    }
    else {
        /* child */
        close(fd[WRITE_END]);

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

        printf("Child process with pid %d\n", getpid());
        printf("Receiving message from parent (pid %d):\n", getppid());
        if ((nbytes = read(fd[READ_END], child_buffer, BUFFER_SIZE)) < 0 ) {
            fprintf(stderr, "Unable to read from pipe: %s\n", strerror(errno));
        }
        close(fd[READ_END]);

        char * str = replaceWord__(child_buffer, keywords[2][0], keywords[2][1]);
        printf("%s", str);

        /*
        token = strtok(child_buffer, "\0");
        //printf("%s\n", token);
        printf("\n\n");
        char *line, *string;
        line = strtok_r(token, "\n", &token);
    
        while(line != NULL) {
            printf("%s\n\n", line);
            string = strtok_r(line, " ", &line);
            while(string != NULL) {
                printf("1: %s ", replaceword(line, keywords, keywords_size));
                string = strtok_r(line, " ", &line);
            }
            line = strtok_r(token, "\n", &token);
        }
        */

        /* write message from parent */
        //write(STDOUT_FILENO, child_buffer, nbytes);
        /* exit gracefully */
        fclose(cypher);
        exit(EXIT_SUCCESS);
    }
}
