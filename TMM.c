/*
CSIT203
Threaded Matrix Multiplication
Abhyuday Singh

NOTE:
    My code is much, much simpler than the others ones I've seen so far, i.e.
    yours and Jeet's. I thought of implementing a verification shell script for
    it, but that would be piggybacking off of Jeet too much.
    Works for all square matrices* but the size of the matrix and its values
    are hardcoded, so that's the only way to change those currently. Also has a
    couple of bad programming practices (code smells) in it and gives the output
    on the compiler instead of a file.
    Still, it works in its current state and I think the purpose of the assignment
    was fulfilled: giving the students familiarity with pthreads and threaded
    processes.

Shortcomings:
    *Datatype used for both input and output are integer types, so larger values 
    can cause overflow. 
    Currently matrix is 3x3

References Used:
    https://www.javatpoint.com/matrix-multiplication-in-c
    https://www.geeksforgeeks.org/multiplication-of-matrix-using-threads/
    https://man7.org/linux/man-pages/man7/pthreads.7.html
    https://docs.oracle.com/cd/E26502_01/html/E35303/tlib-1.html
    Jeet Shah
    And a lot of Stack Overflow
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define N 3  // NxN matrx
#define n 3  // Number of threads

struct v
{
    size_t i;
    size_t j;
};

double A[N][N] = {{1.0, 2.0, 3.0}, {3.0, 4.0, 5.0}, {4.0, 5.0, 6.0}};
double B[N][N] = {{2.0, 3.0, 4.0}, {4.0, 5.0, 6.0}, {2.0, 3.0, 2.0}};
double C[N][N];

static void * matmul(void *arg){
    struct v *data = (struct v *)arg;

    size_t l;
    for(l=0; l < N; l++)
    {
        size_t i = (data[l]).i;
        size_t j = (data[l]).j;
        double sum=0;
        size_t d;

        for (d = 0; d < N; d++)
        {
            sum = sum + A[i][d]*B[d][j];
        }

        C[i][j] = sum;
        sum = 0;
    }
    return 0;
}

int main(void)
{
    pthread_t threads[n];
    size_t i, k;

    struct v **data;
    data = (struct v **)malloc(n * sizeof(struct v*));

    for(i = 0; i < n; i++)
    {
        data[i] = (struct v *)malloc(n * sizeof(struct v));

        for(k = 0; k < n; k++)
        {
            data[i][k].i = i;
            data[i][k].j = k;
        }

        pthread_create(&threads[i], NULL, matmul, data[i]);
    }

    for(i = 0; i < n; i++)
    {
        pthread_join(threads[i], NULL);
    }

    for (i = 0; i < N; i++)
    {
        for (k = 0; k < N; k++)
        {
            printf("%lf\t", C[i][k]);
        }

        printf("\n");

        free(data[i]);
    }
    free(data);
    return 0;
}
