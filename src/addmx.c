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
        printf("file %s could not be read\n", argv[2]);
    }

    char buffer[LINESIZE];
    //fread(buffer, LINESIZE+1, 1, f1);
    //printf("%s\n", buffer);

    int line1, line2, column1, column2;
    char* matrix1_str, *matrix2_str;

    while (!feof(f1)) // to read file
    {
        // function used to read the contents of file
        fread(buffer, sizeof(buffer), 1, f1);
        matrix1_str = buffer;
    }

    while (!feof(f2)) // to read file
    {
        // function used to read the contents of file
        fread(buffer, sizeof(buffer), 1, f2);
        matrix2_str = buffer;
    }

    printf("%s\n", matrix1_str);

    line1 = matrix1_str[0] - '0';
    column1 = matrix1_str[2] - '0';
    line2 = matrix2_str[0] - '0';
    column2 = matrix2_str[2] - '0';

    printf("%d\n", column1);

    
    int matrix1[line1][column1];
    int matrix2[line2][column2];

    rewind(f1);
    fgets(buffer, 4, f1);
    for(int i = 0; i < line1; i++){
        for(int j = 0; j < column1; j++){
            fscanf(f1, "%d ", &matrix1[i][j]);
        }
        fscanf(f1, "\n");
    }

    rewind(f2);
    fgets(buffer, 4, f2);
    for(int i = 0; i < line2; i++){
        for(int j = 0; j < column2; j++){
            fscanf(f2, "%d ", &matrix2[i][j]);
        }
        fscanf(f2, "\n");
    }
    

    
    /*
    char mat_inputs;
    fscanf(f1, "%s", &mat_inputs);
    printf("%s\n", mat_inputs);
    */
    


    // Daqui retira-se o valor da linha e da coluna
    /*
    char * aux1 = strtok(strdup(buffer), "\n");
    char * mat1 = strtok(strdup(aux1), "x");
    line1 = atoi(mat1);
    column1 = atoi(strtok(NULL, "x"));

    int *ptr = mmap ( NULL, line1*column1*sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0 );
    if(ptr == MAP_FAILED){
        printf("Mapping Failed\n");
        return 1;
    }


    */

    /*

    // Daqui retira-se a matriz
    char * line_ = strtok(strdup(buffer), "\n");
    int matrix_val[line1][column1];
    line_  = strtok(NULL, "\n");
    int line_size = 0;
    while(line_) {
        char* rest = line_;
        for(int i = 0; i < column1; i++) {
            char* token = strtok_r(rest, " ", &rest);
            matrix_val[line_size][i] = atoi(token);
        }

        line_  = strtok(NULL, "\n");
        line_size++;
    }

    for(int i = 0; i < 4; i++) {
        for(int k = 0; k < 3; k++)
            printf("%d ", matrix_val[i][k]);
        printf("\n");
    }

    fread(buffer, LINESIZE+1, 1, f2);

    // Daqui retira-se o valor da linha e da coluna
    char * aux2 = strtok(strdup(buffer), "\n");
    char * mat2 = strtok(strdup(aux2), "x");
    line2 = atoi(mat2);
    column2 = atoi(strtok(NULL, "x"));

    // Daqui retira-se a matriz
    char * line = strtok(strdup(buffer), "\n");
    line  = strtok(NULL, "\n");

    int line_size_2 = 0;
    while(line) {
        char* rest = line;
        printf(":%s\n", rest);
        for(int i = 0; i < 3; i++) {
            char* token = strtok_r(rest, " ", &rest);
            //matrix_val[line_size_2][i] = atoi(token);
        }
        printf("\n");
        line  = strtok(NULL, "\n");

        line_size_2++;
    }

    for(int i = 0; i < 4; i++) {
        for(int k = 0; k < 3; k++)
            printf("%d ", matrix_val[i][k]);
        printf("\n");
    }


    

    if(line1 != line2) {
        printf("line have different size: %d != %d\n", line1, line2);
    }
    
    if(column1 != column2) {
        printf("column have different size: %d != %d\n", column1, column2);
    }

    */

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
