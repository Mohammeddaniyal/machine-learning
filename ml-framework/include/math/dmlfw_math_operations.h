#ifndef __DMLFW__MATH_OPERATIONS__
#define __DMLFW__MATH_OPERATIONS__
/**
 * @file dmlfw_math_operations.h
 * @brief Core mathematical vector and matrix functions.
 * @ingroup math_ops
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This module provides functions for common mathematical transformations (sigmoid, log)
 * and reductions (sum) on double precision vectors and matrices, supporting ML and
 * numeric data workflows.
 *
 * Error Handling:
 * ---------------
 * All functions use a centralized error API. After calls, check `dmlfw_error()`
 * and access full details via `dmlfw_get_error_string()` and `dmlfw_get_debug_string()`.
 *
 * Ownership:
 * ----------
 * Functions that allocate new containers (vectors/matrices) transfer ownership to caller.
 * Returned vectors/matrices must be destroyed with the appropriate API function.
 *
 * @example example_dmlfw_math_operations.c
 * Typical usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_math.h>
 * #include <dmlfw_error.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_column_vec_double *v = dmlfw_column_vec_double_create_new(4);
 *     dmlfw_column_vec_double_set(v, 0, 1.0);
 *     dmlfw_column_vec_double_set(v, 1, 2.0);
 *     dmlfw_column_vec_double_set(v, 2, 3.0);
 *     dmlfw_column_vec_double_set(v, 3, 4.0);
 *
 *     dmlfw_column_vec_double *sig = dmlfw_column_vec_double_sigmoid(v, NULL);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error in sigmoid: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_column_vec_double_destroy(v);
 *         return EXIT_FAILURE;
 *     }
 *
 *     double total = dmlfw_column_vec_double_sum(sig);
 *     printf("Sum of sigmoid vector: %lf\n", total);
 *     dmlfw_column_vec_double_destroy(sig);
 *     dmlfw_column_vec_double_destroy(v);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */


#include <dmlfw_vector.h>
#include <dmlfw_matrix.h>

/**
 * @brief Applies sigmoid function element-wise to column vector.
 *
 * If new_vector is NULL, allocates a result container; otherwise, fills provided container.
 * Input and output sizes must match if container is reused.
 *
 * @param vector [in] Source column vector (non-NULL).
 * @param new_vector [in,out] Optional result vector to fill. If NULL, allocated.
 * @return Pointer to result vector, or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *sig = dmlfw_column_vec_double_sigmoid(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in sigmoid: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_column_vec_double_destroy(sig);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_column_vec_double_sigmoid(
    dmlfw_column_vec_double *vector,
    dmlfw_column_vec_double *new_vector);

/**
 * @brief Applies natural logarithm element-wise to column vector.
 *
 * If new_vector is NULL, allocates a new container; otherwise, fills provided container.
 * Input and output sizes must match if container is reused.
 *
 * @param vector [in] Source column vector (non-NULL).
 * @param new_vector [in,out] Optional result vector. If NULL, allocated.
 * @return Pointer to result vector or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *logv = dmlfw_column_vec_double_log(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in log: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_column_vec_double_destroy(logv);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_column_vec_double_log(
    dmlfw_column_vec_double *vector,
    dmlfw_column_vec_double *new_vector);

/**
 * @brief Computes sum of all values in column vector.
 *
 * @param vector [in] Source vector.
 * @return Sum or 0.0 on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double total = dmlfw_column_vec_double_sum(vec);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in sum: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * printf("Sum: %lf\n", total);
 * @endcode
 */
double dmlfw_column_vec_double_sum(dmlfw_column_vec_double *vector);

/**
 * @brief Applies sigmoid function element-wise to a matrix.
 *
 * If new_matrix is NULL, allocates a new matrix; otherwise, fills provided container.
 * Dimensions must match if container is reused.
 *
 * @param matrix [in] Source matrix (non-NULL).
 * @param new_matrix [in,out] Optional result matrix. If NULL, allocated.
 * @return Pointer to result matrix or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *sig_mat = dmlfw_mat_double_sigmoid(mat, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in sigmoid matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(sig_mat);
 * @endcode
 */
dmlfw_mat_double * dmlfw_mat_double_sigmoid(
    dmlfw_mat_double *matrix,
    dmlfw_mat_double *new_matrix);

#endif // __DMLFW__MATH_OPERATIONS__
/** @} */ // end of group
