#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

typedef int element_t;
typedef struct matrix_t
{
    element_t **elements;
    size_t rows;
    size_t cols;
} matrix_t;

typedef struct thread_args_t
{
    const matrix_t *matrix_A;
    const matrix_t *matrix_B;
    matrix_t *matrix_C;
    size_t i;
    size_t j;
} thread_args_t;

void delete_matrix(matrix_t *matrix)
{
    if (matrix == NULL)
        return;

    if (matrix->elements != NULL)
    {
        for (size_t i = 0; i < matrix->rows; i++)
        {
            if (matrix->elements == NULL)
                continue;
            free(matrix->elements[i]);
        }

        free(matrix->elements);
    }

    free(matrix);
}

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
    if (matrix == NULL)
        return;

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

void serial_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C)
{
    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

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

    output_matrix(matrix_C, "Serial calculation - Matrix C:");
}

void *multiple_routine(void *args)
{
    pthread_exit(NULL);
}

void parallel_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C)
{
    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

    size_t num_threads = 0;
    pthread_t *threads = NULL;
    thread_args_t *args = NULL;
    size_t thread_count = 0;

    num_threads = matrix_A->rows * matrix_B->cols;
    threads = calloc(num_threads, sizeof(pthread_t));
    args = calloc(num_threads, sizeof(thread_args_t));
    
    for (size_t i = 0; i < matrix_A->rows; i++)
    {
        for(size_t j = 0; j < matrix_B->cols; j++)
        {
            args[thread_count].matrix_A = matrix_A;
            args[thread_count].matrix_B = matrix_B;
            args[thread_count].matrix_C = matrix_C;

            pthread_create(&threads[thread_count], NULL, multiple_routine, &args[thread_count]);

            thread_count++;
        }
    }

    for (size_t i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    output_matrix(matrix_C, "Parallel calculation - Matrix C:");

    free(args);
    free(threads);
}

int main(int argc, char const *argv[])
{
    size_t n = 9, m = 5, k = 11;
    matrix_t *matrix_A = NULL, *matrix_B = NULL, *matrix_C = NULL;
    size_t num_threads = 0;

    matrix_A = create_matrix(n, m, 1);
    matrix_B = create_matrix(m, k, 1);

    output_matrix(matrix_A, "Matrix A:");
    output_matrix(matrix_B, "Matrix B:");

    matrix_C = create_matrix(n, k, 0);

    // Serial calculation without threads;
    serial_multiply(matrix_A, matrix_B, matrix_C);
    delete_matrix(matrix_C);

    matrix_C = create_matrix(n, k, 0);
    parallel_multiply(matrix_A, matrix_B, matrix_C);
    delete_matrix(matrix_C);

    delete_matrix(matrix_A);
    delete_matrix(matrix_B);

    return 0;
}