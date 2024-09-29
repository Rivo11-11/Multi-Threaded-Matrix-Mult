#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// n is the shared dimension
typedef struct {
    int row;
    int col;
    int n; 
    int** A;
    int** B;
    int** C;
} thread_arg_t;


typedef struct {
    int row;
    int cols2 ;
    int n; 
    int** A;
    int** B;
    int** C;
} row_arg_t;

void multiply_single_thread(int** A, int** B, int** C, int rows1, int cols1, int rows2, int cols2) {
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            C[i][j] = 0;
            for (int k = 0; k < cols1; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


void* compute_element(void* arg) {
    thread_arg_t* data = (thread_arg_t*) arg;
    int sum = 0;
    for (int k = 0; k < data->n; k++) {
        // i fixed the row and column for the element which i want to process   
        sum += data->A[data->row][k] * data->B[k][data->col];
    }
    data->C[data->row][data->col] = sum;
    return NULL;
}


void multiply_element_threads(int** A, int** B, int** C, int rows1, int cols1, int rows2, int cols2) {
    pthread_t threads[rows1][cols2];
    thread_arg_t args[rows1][cols2];

    // create rows1 * cols2 thread each thread responsible for computing 1 element (mean do a 1 row * 1 col calculation)
    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            args[i][j].row = i;
            args[i][j].col = j;
            args[i][j].n = cols1;
            args[i][j].A = A;
            args[i][j].B = B;
            args[i][j].C = C;
            pthread_create(&threads[i][j], NULL, compute_element, &args[i][j]);
        }
    }

    for (int i = 0; i < rows1; i++) {
        for (int j = 0; j < cols2; j++) {
            pthread_join(threads[i][j], NULL);
        }
    }
}


void* compute_row(void* arg) {
    row_arg_t* data = (row_arg_t*) arg;
    for (int j = 0; j < data->cols2; j++) {
        int sum = 0;
        for (int k = 0; k < data->n; k++) {
            // i fixed the row only . and the column will be changing 
            sum += data->A[data->row][k] * data->B[k][j];
        }
        data->C[data->row][j] = sum;
    }
    return NULL;
}


void multiply_row_threads(int** A, int** B, int** C, int rows1, int cols1, int rows2, int cols2) {
    pthread_t threads[rows1];
    row_arg_t args[rows1];

    for (int i = 0; i < rows1; i++) {
        args[i].row = i;
        args[i].cols2 = cols2;
        args[i].n = cols1;
        args[i].A = A;
        args[i].B = B;
        args[i].C = C;
        pthread_create(&threads[i], NULL, compute_row, &args[i]);
    }

    for (int i = 0; i < rows1; i++) {
        pthread_join(threads[i], NULL);
    }
}
void read_matrices(const char* filename, int*** A, int*** B, int* rows1, int* cols1, int* rows2, int* cols2) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        exit(1);
    }

    fscanf(file, "%d %d", rows1, cols1);
    *A = (int**) malloc(*rows1 * sizeof(int*));
    for (int i = 0; i < *rows1; i++) {
        (*A)[i] = (int*) malloc(*cols1 * sizeof(int));
        for (int j = 0; j < *cols1; j++) {
            fscanf(file, "%d", &(*A)[i][j]);
        }
    }

  
    fscanf(file, "%d %d", rows2, cols2);
    *B = (int**) malloc(*rows2 * sizeof(int*));
    for (int i = 0; i < *rows2; i++) {
        (*B)[i] = (int*) malloc(*cols2 * sizeof(int));
        for (int j = 0; j < *cols2; j++) {
            fscanf(file, "%d", &(*B)[i][j]);
        }
    }

    fclose(file);
}


void print_matrix(int** matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}


int main() {
    int **A, **B, **C1, **C2 , **C3 ;
    int rows1, cols1, rows2, cols2;

    // Adjust the test case suitable problem1 => small test case  - problem2 => large test case
    read_matrices("problem2", &A, &B, &rows1, &cols1, &rows2, &cols2);

    if (cols1 != rows2) {
        printf("Matrix multiplication not possible\n");
        return -1;
    }

   
    C1 = (int**) malloc(rows1 * sizeof(int*));
    C2 = (int**) malloc(rows1 * sizeof(int*));
    C3 = (int**) malloc(rows1 * sizeof(int*));
    for (int i = 0; i < rows1; i++) {
        C1[i] = (int*) malloc(cols2 * sizeof(int));
        C2[i] = (int*) malloc(cols2 * sizeof(int));
        C3[i] = (int*) malloc(cols2 * sizeof(int));
    }

     // Perform single thread caculation
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_single_thread(A, B, C3, rows1, cols1, rows2, cols2);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_single = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Result matrix (single-threaded):\n");
    // print_matrix(C3, rows1, cols2);
    printf("END0 [elapsed time: %f seconds]\n", time_single);


    // Perform element-based multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_element_threads(A, B, C1, rows1, cols1, rows2, cols2);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_element = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Result matrix (element-based threading):\n");
    // print_matrix(C1, rows1, cols2);
    printf("END1 [elapsed time: %f seconds]\n", time_element);


    // Perform row-based multiplication
    clock_gettime(CLOCK_MONOTONIC, &start);
    multiply_row_threads(A, B, C2, rows1, cols1, rows2, cols2);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_row = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("Result matrix (row-based threading):\n");
    // print_matrix(C2, rows1, cols2);
    printf("END2 [elapsed time: %f seconds]\n", time_row);

    // Free memory
    for (int i = 0; i < rows1; i++) {
        free(A[i]);
        free(C1[i]);
        free(C2[i]);
        free(C3[i]);
    }
    for (int i = 0; i < rows2; i++) {
        free(B[i]);
    }
    free(A);
    free(B);
    free(C1);
    free(C2);
    free(C3) ;

    return 0;
}
