/*
 * Below is the code that implements matrix 
 * multiplication using multi-threading.
 * This works by calculating each element in
 * a different thread as calculation of one element
 * is not directly dependent on the other.
 * 
 * The code below uses the pthread library is installed by
 * default on most GNU Linux machines. 
 * The file below is compiled using the command below
 * `gcc -Wall -o matrix_multiply matmul.c -lpthread`
 * The file is compiled using gcc 9.4.0 on a Ubuntu 20.04.1 machine.
 * 
 * The matrices here only take signed integers. Hoewever my verifier code
 * makes matrices for doubles. This causes it to not be able to handle very
 * large integers, therefore the test cases have been designed to keep the 
 * input matrix elements in [-2^12, 2^12) integers. This allows for large 
 * test case (such as the last one) to not lead to integer (i guess double) overflow.
 * 
 * The test case where the matrices are not compatible for multiplication
 * has been rigorously tested. The zero dimension matrix hasnt been 
 * tested, an assumption is that a read error or a segfault will occur.
 * 
 * All the randomly generated test cases pass, somewhat proving the 
 * correctness of this program.
 * 
 * For bigger test cases, the largest bottle-neck in terms of time seems to be
 * file i/o -> read, write, compare inputs and outputs.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

// Structure to hold arguments for the matrix multiplication function
typedef struct
{
  int** matrices[2];    // Pointer to matrices
  int** result_matrix;  // Pointer to result matrix
  int size_vector;      // Size of vectors that are multiplied (for each element)
  int result_cols;      // Number of columns in the result matrix
  int start_row;        // Start row for this thread
  int end_row;          // End row for this thread
} multiply_args_t;

// Function to multiply two matrices
void* multiply_matrices(void* args) {
    multiply_args_t* multiply_args = (multiply_args_t*)args;
    int*** matrices = multiply_args->matrices;
    int** result_matrix = multiply_args->result_matrix;
    int size_vector = multiply_args->size_vector;
    int result_cols = multiply_args->result_cols;
    int start_row = multiply_args->start_row;
    int end_row = multiply_args->end_row;

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < result_cols; j++)
        {
            result_matrix[i][j] = 0;
            for (int k = 0; k < size_vector; k++)
            {
                result_matrix[i][j] += matrices[0][i][k] * matrices[1][k][j];
            }
        }
    }

    return NULL;
}

size_t count_columns(char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", file_name);
        return -1;
    }

    size_t count = 0;
    int in_word = 0;
    int c;

    while ((c = fgetc(fp)) != EOF && c != '\n')
    {
        if (isspace(c))
        {
            in_word = 0;
        }
        else if (!in_word)
        {
            in_word = 1;
            count++;
        }
    }

    fclose(fp);
    return count;
}

size_t count_rows(char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", file_name);
        return -1;
    }

    size_t count = 0;
    int c;

    while ((c = fgetc(fp)) != EOF)
    {
        if (c == '\n')
        {
            count++;
        }
    }

    if (count > 0 || ftell(fp) == 0)
    {
        // If the file is not empty, or if the last line has a newline character,
        // then we need to count that as an additional line.
        count++;
    }

    fclose(fp);
    // assuming there is an empty newline for each file
    // we are working on that assumption -> formatting will be specified
    return count - 1;
}

int **allocate_matrix(size_t rows, size_t cols)
{
    // Allocate memory for the matrix
    int **matrix = (int **)malloc(rows * sizeof(int *));
    if (matrix == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for matrix\n");
        return NULL;
    }
    for (size_t i = 0; i < rows; i++)
    {
        matrix[i] = (int *)malloc(cols * sizeof(int));
        if (matrix[i] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for matrix row %lu\n", i);
            for (size_t j = 0; j < i; j++)
            {
                free(matrix[j]);
            }
            free(matrix);
            return NULL;
        }
    }

    return matrix;
}

void free_matrix(int **matrix, size_t rows)
{
    for (size_t i = 0; i < rows; i++) 
    {
        free(matrix[i]);
    }
    free(matrix);
}

int read_matrix_from_file(int **matrix, size_t rows, size_t cols, char *file_name)
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", file_name);
        return -1;
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            if (fscanf(fp, "%d", &matrix[i][j]) != 1)
            {
                fprintf(stderr, "Failed to read element at row %lu, column %lu from file %s\n", i, j, file_name);
                fclose(fp);
                return -1;
            }
        }
        // Read and discard the newline character at the end of the row
        if (fgetc(fp) != '\n')
        {
            fprintf(stderr, "Expected newline character at end of row %lu in file %s\n", i, file_name);
            fclose(fp);
            return -1;
        }
    }

    fclose(fp);
    return 0;
}

void print_console_mat(int **mat, size_t rows, size_t cols)
{
    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
}

int print_file_mat(char *file_name, int **mat, size_t rows, size_t cols)
{
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        fprintf(stderr, "Failed to open file %s\n", file_name);
        return -1;
    }

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
        {
            fprintf(fp, "%d ", mat[i][j]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    return 0;
}


int main(int argc, char **argv)
{
    if (argc != 5)
    {
        fprintf(stderr, "Usage: %s matrixA.txt matrixB.txt result.txt num_threads\n", argv[0]);
        exit(1);
    }

    char *file_a = argv[1];
    char *file_b = argv[2];
// allocating memory for matrices
    size_t a_col = count_columns(file_a);
    size_t b_col = count_columns(file_b);
    size_t a_row = count_rows(file_a);
    size_t b_row = count_rows(file_b);

    if (a_col != b_row)
    {
        printf("Dimensions:\nA -> %ld x %ld\nB -> %ld x %ld\n", a_row, a_col, b_row, b_col);
        fprintf(stderr, "Error: matrix dimensions do not match\n");
        exit(1);
    }

    int **matA = allocate_matrix(a_row, a_col);
    read_matrix_from_file(matA, a_row, a_col, file_a);

    int **matB = allocate_matrix(b_row, b_col);
    read_matrix_from_file(matB, b_row, b_col, file_b);

    /*************debugging*************/
    // print_console_mat(matA, a_row, a_col); printf("\n");
    // print_console_mat(matB, b_row, b_col);
    /*************debugging*************/

    // Create result matrix
    int **result = allocate_matrix(a_row, b_col);

    // Create threads and assign work
    int num_threads = a_row < atoi(argv[4]) ? a_row : atoi(argv[4]);

    // Create the thread pool
    pthread_t threads[num_threads];

    // Create arguments for the matrix multiplication function
    multiply_args_t multiply_args[num_threads];

    int rows_per_thread = (a_row + num_threads - 1) / num_threads;
    for (int i = 0; i < num_threads; i++) {
        multiply_args[i].matrices[0] = matA;
        multiply_args[i].matrices[1] = matB;
        multiply_args[i].result_matrix = result;
        multiply_args[i].size_vector = b_row;
        multiply_args[i].result_cols = b_col;
        multiply_args[i].start_row = i * rows_per_thread;
        multiply_args[i].end_row = (i + 1) * rows_per_thread;

        if (multiply_args[i].end_row > a_row) {
            multiply_args[i].end_row = a_row;
        }
    }

// Multiply the matrices using multiple threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, multiply_matrices, &multiply_args[i]);
    }

// Wait for all threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }


    /*************debugging*************/
    // print_console_mat(result, a_row, b_col); 
    // printf("%s\n", argv[3]);
    /*************debugging*************/

    print_file_mat(argv[3], result, a_row, b_col);
    
    free_matrix(matA, a_row);
    free_matrix(matB, b_row);
    free_matrix(result, a_row);

    return 0;
}
