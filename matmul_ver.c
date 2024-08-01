/*
 * Below is the code that uses the GNU Scientific Library
 * to multiply matrices, this will be used as verification
 * for the result of my multi-threaded matrix multiplication program.
 * 
 * To compile this, gsl library must be installed in the
 * default location, if one is unsure about that, can install
 * using the command below (on Debian architecture)
 * `sudo apt-get install libgsl-dev`
 * The file below is compiled using the command below
 * `gcc -Wall -o matrix_multiply_verify matmul_ver.c -lgsl -lgslcblas`
 * The file is compiled using gcc 9.4.0 on a Ubuntu 20.04.1 machine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_matrix.h>

void print_console_gslmat(const gsl_matrix *m)
{
    for (int i = 0; i < m->size1; ++i)
    {
        for (int j = 0; j < m->size2; ++j)
        {
            printf("%g ", gsl_matrix_get(m, i, j));
        }
        printf("\n");
    }
    printf("\n");
}

int print_file_gslmat(FILE *f, const gsl_matrix *m)
{
    int status, n = 0;

    for (size_t i = 0; i < m->size1; i++)
    {
        for (size_t j = 0; j < m->size2; j++)
        {
            if ((status = fprintf(f, "%.0lf ", gsl_matrix_get(m, i, j))) < 0)
            {
                return -1;
            }
            n += status;
        }

        if ((status = fprintf(f, "\n")) < 0)
        {
            return -1;
        }
        n += status;
    }

    return n;
}

int count_columns(char *file_name)
{
    FILE *fp = fopen(file_name, "r");

    int count = 0;
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

int count_rows(char *file_name)
{
    FILE *fp = fopen(file_name, "r");

    int count = 0;
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
    return count - 1;
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s <matrix_a_file> <matrix_b_file> <result_file>\n", argv[0]);
        exit(1);
    }

    char *file_a = argv[1];
    char *file_b = argv[2];

    // allocating memory for matrices
    int a_col = count_columns(file_a);
    int b_col = count_columns(file_b);

    int a_row = count_rows(file_a);
    int b_row = count_rows(file_b);

    gsl_matrix *mat_a = gsl_matrix_alloc(a_row, a_col);
    gsl_matrix *mat_b = gsl_matrix_alloc(b_row, b_col);

    // building gsl matrices
    FILE *file_a_ptr = fopen(argv[1], "r");
    FILE *file_b_ptr = fopen(argv[2], "r");

    gsl_matrix_fscanf(file_a_ptr, mat_a);
    gsl_matrix_fscanf(file_b_ptr, mat_b);

    fclose(file_a_ptr);
    fclose(file_b_ptr);

    /*************debugging*************/
    // print_console_gslmat(mat_a); printf("\n");
    // print_console_gslmat(mat_b);
    /*************debugging*************/

    // Perform matrix multiplication
    gsl_matrix *result = gsl_matrix_alloc(mat_a->size1, mat_b->size2);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, mat_a, mat_b, 0.0, result);

    // Write result to output file
    FILE *result_file = fopen(argv[3], "w");
    if (!result_file)
    {
        fprintf(stderr, "Error: could not open output file\n");
        exit(1);
    }

    print_file_gslmat(result_file, result);
    fclose(result_file);

    // Free memory
    gsl_matrix_free(mat_a);
    gsl_matrix_free(mat_b);
    gsl_matrix_free(result);

    return 0;
}
