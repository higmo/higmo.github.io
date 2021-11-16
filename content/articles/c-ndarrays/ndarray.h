/**
 * Author: Simon Guillot <[first name].[last name]@epita.fr>
 *
 * Preprocessor helpers for N-dimensional array types
 *
 * Usage example:
 *
 * #include <stdio.h>
 * #include <ndarray.h>
 * 
 * typedef NDARRAY_TYPE(float, 2) NDArray2f;
 * typedef NDARRAY_TYPE(float, 3) NDArray3f;
 * 
 * int main() {
 *     NDArray2f* A = NEW_NDARRAY(float, 4, 4);    // alloc a 4x4 array
 *     NDArray3f* B = NEW_NDARRAY(float, 3, 3, 3); // alloc a 3x3x3 array
 * 
 *     NDAT(A, 0, 1) = 42;   // access element at position (0, 1) of A
 *     NDAT(B, 0, 1, 1) = 7; // access element at position (0, 1, 1) of B
 * 
 *     printf("%f\n%f\n", NDAT(A, 0, 1), NDAT(B, 0, 1, 1));
 * 
 *     free(A);
 *     free(B);
 * 
 *     return 0;
 * }
*/

#pragma once

#include <stdarg.h>
#include <stdlib.h>

// PREPROC_NARG is expanded to the number of its arguments
// This is a simplified version of Laurent Deniau's method
// <https://groups.google.com/d/msg/comp.std.c/d-6Mj5Lko_s/5R6bMWTEbzQJ>
#define PREPROC_NARG(...) PREPROC_ARG_N(__VA_ARGS__, 4, 3, 2, 1, 0)
#define PREPROC_ARG_N(_1, _2, _3, _4, N, ...) N

// Macro dispatcher
#define NDARRAY_DISPATCH(METHOD, DIM, ...) \
    NDARRAY_ ## METHOD ## _ ## DIM ## D(__VA_ARGS__)

/*************
 * Base type *
 *************/

/**
 * Defines an anonymous matrix struct
 **/
#define NDARRAY_TYPE(TYPE, DIM) \
    struct {                    \
        size_t size[DIM];       \
        TYPE data[];            \
    }

/**
 * Useful for forward declarations
 * The struct name is in format _matrix_[dimensions]d_[type]
 * For instance, NDARRAY_DECL(float, 2) defines a struct named _matrix_2d_float
 **/
#define NDARRAY_DECL(TYPE, DIM)      \
    struct _matrix_##DIM##d_##TYPE { \
        size_t size[DIM];            \
        TYPE data[];                 \
    }

/****************
 * Constructors *
 ****************/

#define NEW_NDARRAY(TYPE, ...)                                   \
    init_matrix_head(NDARRAY_ALLOC(TYPE, __VA_ARGS__),           \
                     PREPROC_NARG(__VA_ARGS__), __VA_ARGS__)

#define NDARRAY_ALLOC(TYPE, ...)                                         \
    NDARRAY_ALLOC_EXPAND(PREPROC_NARG(__VA_ARGS__),                      \
                         NDARRAY_TYPE(TYPE, PREPROC_NARG(__VA_ARGS__)),   \
                         TYPE, __VA_ARGS__)

#define NDARRAY_ALLOC_EXPAND(DIM, ...) \
    NDARRAY_DISPATCH(ALLOC, DIM, __VA_ARGS__)

#define NDARRAY_ALLOC_1D(NDARRAY_TYPE, TYPE, X)    \
    malloc(sizeof(NDARRAY_TYPE) + X * sizeof(TYPE))

#define NDARRAY_ALLOC_2D(NDARRAY_TYPE, TYPE, X, Y)    \
    malloc(sizeof(NDARRAY_TYPE) + X * Y * sizeof(TYPE))

#define NDARRAY_ALLOC_3D(NDARRAY_TYPE, TYPE, X, Y, Z) \
    malloc(sizeof(NDARRAY_TYPE) + X * Y * Z * sizeof(TYPE))

#define NDARRAY_ALLOC_4D(NDARRAY_TYPE, TYPE, X, Y, Z, W) \
    malloc(sizeof(NDARRAY_TYPE) + X * Y * Z * W * sizeof(TYPE))

static inline void* init_matrix_head(void* matrix, const int dim, ...) {
    size_t* head = (size_t*)matrix;

    va_list ap;

    va_start(ap, dim);

    for (int i = 0; i < dim; i++)
        head[i] = va_arg(ap, size_t);

    va_end(ap);

    return matrix;
}

/*************
 * Accessors *
 *************/

// NDAT is expanded depending on its number of arguments
#define NDAT(NDARRAY, ...) \
    NDARRAY_INDEX_EXPAND(PREPROC_NARG(__VA_ARGS__), NDARRAY, __VA_ARGS__)

#define NDARRAY_INDEX_EXPAND(DIM, ...) \
    NDARRAY_DISPATCH(INDEX, DIM, __VA_ARGS__)

#define NDARRAY_INDEX_1D(NDARRAY, X) \
    NDARRAY->data[X]

#define NDARRAY_INDEX_2D(NDARRAY, X, Y) \
    NDARRAY->data[X * NDARRAY->size[1] + Y]

#define NDARRAY_INDEX_3D(NDARRAY, X, Y, Z) \
    NDARRAY->data[X * NDARRAY->size[1] * NDARRAY->size[2] \
                + Y * NDARRAY->size[2] + Z]

#define NDARRAY_INDEX_4D(NDARRAY, X, Y, Z, W) \
    NDARRAY->data[X * NDARRAY->size[1] * NDARRAY->size[2] * NDARRAY->size[3] \
                + Y * NDARRAY->size[2] * NDARRAY->size[3] \
                + Z * NDARRAY->size[3] + W]
