# Matrix Multiplication with Multi-Threading

This project implements matrix multiplication using multi-threading in C. The implementation compares three different methods of matrix multiplication:

1. **Single-threaded**: A single thread handles the multiplication of the entire matrix.
2. **Thread per Element**: Each element of the resulting matrix is computed by a separate thread.
3. **Thread per Row**: Each row of the resulting matrix is computed by a separate thread.

## Performance Comparison

The performance of the three methods was tested with varying matrix sizes. The results show:

- For smaller matrices, the **single-threaded** approach is faster than the multi-threaded methods.
- The **row-threaded** approach performs better than the **thread per element** method for both small and large matrices.
- For larger matrices, the **row-threaded** method exhibits the best performance.

## Matrix Generation Script

The project includes a script (`script.c`) that generates larger matrices to facilitate performance comparison. You can use this script to create test cases with different dimensions,
or you can use the test cases provided problem1 (small size matrix) & problem2 (large size matrix)

## Compilation and Execution

To compile the project, follow these steps:

1. Open a terminal.
2. Navigate to the directory containing the `matrix.c` file.
3. choose the test case you want to apply on problem1 or problem2 when reading the file
4. Compile the code using the following command:

   ```bash
   gcc matrix.c -o executable -lpthread
   ./executable

