#include "matrix.h"
#include <stdlib.h>

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