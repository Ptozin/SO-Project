#include <sys/mman.h>
#include <stdio.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define READ_END 0
#define WRITE_END 1
#define LINESIZE 256

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("usage: addmx file1 file2\n");
        return EXIT_FAILURE;
    }

    FILE * f1 = fopen(argv[1], "r");
    if(!f1) {
        printf("file %s could not be read\n", argv[1]);
        return EXIT_FAILURE;
    }
    FILE * f2 = fopen(argv[2], "r");
    if(!f2) {
        fclose(f1);
        printf("file %s could not be read\n", argv[2]);
        return EXIT_FAILURE;
    }

    char buffer[LINESIZE];

    int line1, line2, column1, column2;

    // to read line and column of matrix1
    fscanf(f1, "%dx%d", &line1, &column1);
    // to read line and column of matrix2
    fscanf(f2, "%dx%d", &line2, &column2);

    if(line1 != line2 || column1 != column2){
        printf("Matrices have different sizes\n");
        fclose(f1);
        fclose(f2);
        return EXIT_FAILURE;
    }

    int *ptr = mmap(NULL, 3*line1*column1*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0 );
    if(ptr == MAP_FAILED){
        perror("Mapping Failed. Check if the first line of the file is of type \"n\"x\"m\" ");
        fclose(f1);
        fclose(f2);
        return EXIT_FAILURE;
    }

    
    rewind(f1);
    fgets(buffer, LINESIZE, f1);
    for(int i = 0; i < line1; i++){
        for(int j = 0; j < column1; j++){
            if(!fscanf(f1, "%d", &ptr[i*column1+j])){
                printf("Error while loading matrix1. Check if number of lines and columns correspond with the first line.\n");
                fclose(f1);
                fclose(f2);
                return EXIT_FAILURE;
            }
            if(i == line1-1 && j == column1-1) {
                if(!feof(f1)) {
                    printf("Error while loading matrix1. Make sure there are no newlines at the end of the file.\nCheck if number of lines and columns correspond with the first line.\n");
                    fclose(f1);
                    fclose(f2);
                    return EXIT_FAILURE;
                }
            }
            else if(feof(f1)) {
                printf("Error while loading matrix1. Check if number of lines and columns correspond with the first line.\n");
                fclose(f1);
                fclose(f2);
                return EXIT_FAILURE;
            }
        }
        fscanf(f1, "\n");
    }

    rewind(f2);
    fgets(buffer, LINESIZE, f2);
    for(int i = 0; i < line1; i++){
        for(int j = 0; j < column1; j++){
            if(!fscanf(f2, "%d", &ptr[line1*column1 + i*column2+j])){
                printf("Error while loading matrix2. Check if number of lines and columns correspond with the first line.\n");
                fclose(f1);
                fclose(f2);
                return EXIT_FAILURE;
            }
            if(i == line1-1 && j == column1-1) {
                if(!feof(f2)) {
                    printf("Error while loading matrix2. Make sure there are no newlines at the end of the file.\nCheck if number of lines and columns correspond with the first line.\n");
                    fclose(f1);
                    fclose(f2);
                    return EXIT_FAILURE;
                }
            }
            else if(feof(f2)) {
                printf("Error while loading matrix2. Check if number of lines and columns correspond with the first line.\n");
                fclose(f1);
                fclose(f2);
                return EXIT_FAILURE;
            }
        }
        fscanf(f2, "\n");
    }


    for(int i = 1; i <= column1; i++){
        pid_t pid;
        if ((pid = fork()) == -1) {
            perror("Fork Error");
            fclose(f1);
            fclose(f2);
            return EXIT_FAILURE;
        }
        else if (pid == 0) {
            /* child process */
            for(int j = 1; j <= line1*column1; j++){
                if(j % column1 == i || (i == column1 && j % column1 == 0)){
                    *(ptr + (j + (line1 * column1) * 2) - 1) = *(ptr + (j + (line1 * column1)) - 1) + *(ptr + j - 1);
                }
            }
            return EXIT_SUCCESS;
        }
        else {
            /* parent process */
            if (waitpid(pid, NULL, 0) == -1) {
                perror("Wait Error\n");
                fclose(f1);
                fclose(f2);
                return EXIT_FAILURE;
            }
        }
    }

    printf("%dx%d", line1, column1);
    for(int i = line1*column1*2; i < line1*column1*3; i++) {
        if(i%column1 == 0) printf("\n");
        printf("%d ",*(ptr + i));
    }
    printf("\n");


    fclose(f1);
    fclose(f2);
    return EXIT_SUCCESS;
}