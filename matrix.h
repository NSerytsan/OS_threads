#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#define DEFAULT_N 100
#define DEFAULT_M 100000
#define DEFAULT_K 50
#define DEFAULT_NUM_THREADS 4

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
    size_t start_row;
    size_t end_row;
} thread_args_t;

void delete_matrix(matrix_t *matrix);
matrix_t *create_matrix(size_t rows, size_t cols, int autofill);

#endif