#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef int element_t;
typedef element_t **elements_t;
typedef struct matrix_t
{
    elements_t elements;
    size_t rows;
    size_t cols;
} matrix_t;

matrix_t *create_matrix(size_t rows, size_t cols, int autofill)
{
    matrix_t *matrix = calloc(1, sizeof(matrix_t));

    matrix->rows = rows;
    matrix->cols = cols;
    matrix->elements = calloc(matrix->rows, sizeof(element_t *));

    for (size_t i = 0; i < matrix->rows; ++i)
    {
        matrix->elements[i] = calloc(matrix->cols, sizeof(element_t));
        if (autofill)
        {
            for (size_t j = 0; j < matrix->cols; ++j)
            {
                matrix->elements[i][j] = rand() % 10;
            }
        }
    }

    return matrix;
}

void output_matrix(const matrix_t *matrix, char *title)
{
    if (title != NULL)
    {
        printf("%s\n", title);
    }

    for (size_t i = 0; i < matrix->rows; i++)
    {
        for (size_t j = 0; j < matrix->cols; j++)
        {
            printf(" %3d", matrix->elements[i][j]);
        }
        printf("\n");
    }
}

matrix_t *serial_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B)
{
    matrix_t *matrix_C = NULL;
    if (matrix_A == NULL || matrix_B == NULL || matrix_A->cols != matrix_B->rows)
    {
        return NULL;
    }

    matrix_C = create_matrix(matrix_A->rows, matrix_B->cols, 0);

    for (size_t i = 0; i < matrix_A->rows; i++)
    {
        for (size_t j = 0; j < matrix_B->cols; j++)
        {
            for (size_t k = 0; k < matrix_A->cols; k++)
            {
                matrix_C->elements[i][j] += matrix_A->elements[i][k] * matrix_B->elements[k][j];
            }
        }
    }

    return matrix_C;
}

void *start_routine(void *args)
{
    pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
    size_t n = 5, m = 4, k = 6;
    matrix_t *matrix_A = NULL, *matrix_B = NULL, *matrix_C = NULL;
    size_t num_threads = 0;

    // Create and fill matrixes
    matrix_A = create_matrix(n, m, 1);
    matrix_B = create_matrix(m, k, 1);

    // Output matrixes
    output_matrix(matrix_A, "Matrix A:");
    output_matrix(matrix_B, "Matrix B:");

    // Serial calculation without threads;
    matrix_C = serial_multiply(matrix_A, matrix_B);
    output_matrix(matrix_C, "Serial calculation - Matrix C:");

    return 0;
}