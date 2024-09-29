#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_matrix(int rows, int cols, FILE* file) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%d ", rand() % 10);  // Random numbers between 0 and 9
        }
        fprintf(file, "\n");
    }
}

int main() {
    srand(time(NULL));  

    // Define matrix dimensions
    int rowsA = 150, colsA = 100;
    int rowsB = 100, colsB = 150;

 
    FILE* file = fopen("problem2", "w");
    if (!file) {
        perror("Failed to open file");
        return 1;
    }

    fprintf(file, "%d %d\n", rowsA, colsA);
    generate_matrix(rowsA, colsA, file);

    fprintf(file, "%d %d\n", rowsB, colsB);
    generate_matrix(rowsB, colsB, file);

    fclose(file);
    return 0;
}
