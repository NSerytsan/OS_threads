#include "matrix.h"

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

void *multiply_routine(void *args)
{
    thread_args_t *thread_args = args;

    for (size_t i = thread_args->start_row; i < thread_args->end_row; i++)
    {
        for (size_t j = 0; j < thread_args->matrix_B->cols; j++)
        {
            element_t result = 0;
            for (size_t k = 0; k < thread_args->matrix_A->cols; k++)
            {
                result += thread_args->matrix_A->elements[i][k] * thread_args->matrix_B->elements[k][j];
            }
            thread_args->matrix_C->elements[i][j] = result;
            printf("[%3ld,%3ld] = %d\n", i, j, result);
        }
    }

    pthread_exit(NULL);
}

void parallel_multiply(const matrix_t *matrix_A, const matrix_t *matrix_B, matrix_t *matrix_C, size_t num_threads)
{
    struct timeval start, end;
    double exec_time = 0;
    pthread_t *threads = NULL;
    thread_args_t *args = NULL;

    if (matrix_A == NULL || matrix_B == NULL || matrix_C == NULL || matrix_A->cols != matrix_B->rows)
        return;

    if (matrix_A->rows < num_threads)
        num_threads = matrix_A->rows;

    threads = calloc(num_threads, sizeof(pthread_t));
    args = calloc(num_threads, sizeof(thread_args_t));
    size_t num_row_per_tread = matrix_A->rows / num_threads;

    gettimeofday(&start, NULL);

    for (size_t i = 0; i < num_threads; i++)
    {
        args[i].matrix_A = matrix_A;
        args[i].matrix_B = matrix_B;
        args[i].matrix_C = matrix_C;
        args[i].start_row = i * num_row_per_tread;
        args[i].end_row = args[i].start_row + num_row_per_tread;
        pthread_create(&threads[i], NULL, multiply_routine, &args[i]);
    }

    for (size_t i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end, NULL);
    exec_time = ((end.tv_sec - start.tv_sec) * 1000.0) + (end.tv_usec - start.tv_usec) / 1000.0;
    printf("Calculation time: %.3lf ms\n", exec_time);

    free(args);
    free(threads);
}

int main(int argc, char const *argv[])
{
    size_t n = DEFAULT_N, m = DEFAULT_M, k = DEFAULT_K;
    matrix_t *matrix_A = NULL, *matrix_B = NULL, *matrix_C = NULL;
    size_t num_threads = DEFAULT_NUM_THREADS;

    matrix_A = create_matrix(n, m, 1);
    matrix_B = create_matrix(m, k, 1);
    matrix_C = create_matrix(n, k, 0);

    parallel_multiply(matrix_A, matrix_B, matrix_C, num_threads);
    delete_matrix(matrix_C);

    delete_matrix(matrix_A);
    delete_matrix(matrix_B);

    return 0;
}