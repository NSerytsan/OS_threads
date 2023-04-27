#include "matrix.h"

#include <stdio.h>
#include <sys/time.h>

void serial_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C)
{
    struct timeval start, end;
    double exec_time = 0;

    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

    gettimeofday(&start, NULL);

    for (size_t i = 0; i < matrix_A->rows; i++)
    {
        for (size_t j = 0; j < matrix_B->cols; j++)
        {
            for (size_t k = 0; k < matrix_A->cols; k++)
            {
                matrix_C->elements[i][j] +=
                    matrix_A->elements[i][k] * matrix_B->elements[k][j];
            }
            printf("[%3ld,%3ld] = %d\n", i, j, matrix_C->elements[i][j]);
        }
    }

    gettimeofday(&end, NULL);
    exec_time = ((end.tv_sec - start.tv_sec) * 1000.) + (end.tv_usec - start.tv_usec) / 1000.0;

    printf("Calculation time: %.3lf ms\n", exec_time);
}

int main(int argc, char const *argv[])
{
    size_t n = DEFAULT_N, m = DEFAULT_M, k = DEFAULT_K;
    matrix_t *matrix_A = NULL, *matrix_B = NULL, *matrix_C = NULL;

    matrix_A = create_matrix(n, m, 1);
    matrix_B = create_matrix(m, k, 1);
    matrix_C = create_matrix(n, k, 0);

    serial_multiply(matrix_A, matrix_B, matrix_C);
    delete_matrix(matrix_C);

    delete_matrix(matrix_A);
    delete_matrix(matrix_B);

    return 0;
}