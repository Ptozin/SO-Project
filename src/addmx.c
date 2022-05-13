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
    }
    FILE * f2 = fopen(argv[2], "r");
    if(!f2) {
        fclose(f1);
        printf("file %s could not be read\n", argv[2]);
    }

    char buffer[LINESIZE];

    int line1, line2, column1, column2;

    // to read line and column of matrix
    fscanf(f1, "%dx%d", &line1, &column1);
    // to read line and column of matrix
    fscanf(f2, "%dx%d", &line2, &column2);
     
    int matrix1[line1][column1];
    int matrix2[line2][column2];

    rewind(f1);
    fgets(buffer, LINESIZE, f1);
    for(int i = 0; i < line1; i++){
        for(int j = 0; j < column1; j++){
            fscanf(f1, "%d ", &matrix1[i][j]);
        }
        fscanf(f1, "\n");
    }

    rewind(f2);
    fgets(buffer, LINESIZE, f2);
    for(int i = 0; i < line2; i++){
        for(int j = 0; j < column2; j++){
            fscanf(f2, "%d ", &matrix2[i][j]);
        }
        fscanf(f2, "\n");
    }


    int *ptr = mmap ( NULL, line1*column1*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0 );
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }

    fclose(f1);
    fclose(f2);
    return EXIT_SUCCESS;
}

/*
sacar n = argv[1][0], m = argv[1][2]
fazer for(i < n)
    fork()
    processo filho:
    for(j < m)
        matriz1[i] + matriz2[i]
        guardar resultado

    matar o filho
dar print 
*/
