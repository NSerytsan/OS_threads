#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

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

void serial_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C)
{
    struct timeval start, end;
    double exec_time = 0;

    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

    printf("\nSerial calculation:\n");

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
    
    printf("Serial calculation time: %.3lf ms\n", exec_time);
}

void *multiply_routine(void *args)
{
    thread_args_t *thread_args = args;

    for (size_t k = 0; k < thread_args->matrix_A->cols; k++)
    {
        thread_args->matrix_C->elements[thread_args->i][thread_args->j] +=
            thread_args->matrix_A->elements[thread_args->i][k] * thread_args->matrix_B->elements[k][thread_args->j];
    }

    printf("[%3ld,%3ld] = %d\n", thread_args->i, thread_args->j, thread_args->matrix_C->elements[thread_args->i][thread_args->j]);
    pthread_exit(NULL);
}

void parallel_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C)
{
    struct timeval start, end;
    double exec_time = 0;
    size_t num_threads = 0;
    pthread_t *threads = NULL;
    thread_args_t *args = NULL;
    size_t thread_count = 0;

    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

    num_threads = matrix_A->rows * matrix_B->cols;
    threads = calloc(num_threads, sizeof(pthread_t));
    args = calloc(num_threads, sizeof(thread_args_t));

    printf("\nParallel calculation:\n");
    
    gettimeofday(&start, NULL);
    
    for (size_t i = 0; i < matrix_A->rows; i++)
    {
        for (size_t j = 0; j < matrix_B->cols; j++)
        {
            args[thread_count].matrix_A = matrix_A;
            args[thread_count].matrix_B = matrix_B;
            args[thread_count].matrix_C = matrix_C;
            args[thread_count].i = i;
            args[thread_count].j = j;

            pthread_create(&threads[thread_count], NULL, multiply_routine, &args[thread_count]);

            thread_count++;
        }
    }

    for (size_t i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    gettimeofday(&end, NULL);
    exec_time = ((end.tv_sec - start.tv_sec) * 1000.0) + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Parallel calculation time: %.3lf ms\n", exec_time);

    free(args);
    free(threads);
}

int main(int argc, char const *argv[])
{
    size_t n = 20, m = 1000000, k = 10;
    matrix_t *matrix_A = NULL, *matrix_B = NULL, *matrix_C = NULL;
    size_t num_threads = 0;

    matrix_A = create_matrix(n, m, 1);
    matrix_B = create_matrix(m, k, 1);

    matrix_C = create_matrix(n, k, 0);

    // Serial calculation without threads;
    serial_multiply(matrix_A, matrix_B, matrix_C);
    delete_matrix(matrix_C);

    matrix_C = create_matrix(n, k, 0);
    // Parallel calculation with n*k threads
    parallel_multiply(matrix_A, matrix_B, matrix_C);
    delete_matrix(matrix_C);

    delete_matrix(matrix_A);
    delete_matrix(matrix_B);

    return 0;
}