 #ifndef __DMLFW__OPERATIONS__DOUBLE__
 #define __DMLFW__OPERATIONS__DOUBLE__
/**
 * @file dmlfw_operations_double.h
 * @defgroup operations_double Double Precision Arithmetic
 * @ingroup operations
 * @brief Core double precision vector and matrix arithmetic operations.
 *
 * Version: 1.0
 * Date: 2025-09-25
 *
 * @{
 * This module provides fundamental arithmetic operations specialized for double precision
 * vectors and matrices, including multiplication, addition, subtraction, scalar scaling,
 * and advanced matrix multiplication algorithms like Strassen’s method.
 *
 * Error Handling:
 * ---------------
 * Functions validate all input pointers and vector/matrix dimensions.
 * Errors are reported through a centralized error API.
 * After calling functions, check `dmlfw_error()` and retrieve error details via
 * `dmlfw_get_error_string()` and `dmlfw_get_debug_string()`.
 *
 * Ownership:
 * ----------
 * Inputs are owned by the caller. Functions allocate new output vectors/matrices if
 * corresponding pointers are `NULL`. Any allocated output vectors/matrices are owned by
 * the caller who must free them.
 * @example example_dmlfw_operations_double.c
 * Usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_vector.h>
 * #include <dmlfw_operations.h>
 * #include <dmlfw_error.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *
 *     // Create row vector of size 3
 *     dmlfw_row_vec_double *row_vec = dmlfw_row_vec_double_create_new(3);
 *     dmlfw_row_vec_double_set(row_vec, 0, 1.0);
 *     dmlfw_row_vec_double_set(row_vec, 1, 2.0);
 *     dmlfw_row_vec_double_set(row_vec, 2, 3.0);
 *
 *     // Create column vector of size 3
 *     dmlfw_column_vec_double *col_vec = dmlfw_column_vec_double_create_new(3);
 *     dmlfw_column_vec_double_set(col_vec, 0, 4.0);
 *     dmlfw_column_vec_double_set(col_vec, 1, 5.0);
 *     dmlfw_column_vec_double_set(col_vec, 2, 6.0);
 *
 *     // Multiply row vector with column vector (dot product)
 *     dmlfw_column_vec_double *product = dmlfw_multiply_double_row_vector_with_column_vector(row_vec, col_vec, NULL);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error multiplying vectors: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     printf("Dot product result: %lf\n", dmlfw_column_vec_double_get(product, 0));
 *
 *     dmlfw_column_vec_double_destroy(product);
 *     dmlfw_row_vec_double_destroy(row_vec);
 *     dmlfw_column_vec_double_destroy(col_vec);
 *
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */



/**
 * @brief Multiplies a row vector with a column vector, producing a single-value column vector.
 *
 * @param left_vector [in] Left row vector (non-NULL).
 * @param right_vector [in] Right column vector (non-NULL).
 * @param product_vector [in,out] Optional pre-allocated output vector, size must be 1.
 *                               If NULL, a new vector is allocated.
 * @return Pointer to the resulting single-value column vector or NULL on error.
 *
 * @note Input vectors must have the same size.
 *
 * @see dmlfw_multiply_double_matrix_with_column_vector
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *result = dmlfw_multiply_double_row_vector_with_column_vector(row_vec, col_vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error multiplying vectors: %s\nDebug info: %s\n", err, dbg);
 *     // Cleanup if needed
 *     return NULL;
 * }
 * // Use result...
 * dmlfw_column_vec_double_destroy(result);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_multiply_double_row_vector_with_column_vector(
    dmlfw_row_vec_double *left_vector,
    dmlfw_column_vec_double *right_vector,
    dmlfw_column_vec_double *product_vector);

/**
 * @brief Multiplies a matrix with a column vector, producing a column vector.
 *
 * @param left_matrix [in] Input matrix (non-NULL).
 * @param right_vector [in] Column vector (non-NULL).
 * @param product_vector [in,out] Optional output vector with size matching matrix rows.
 *                               If NULL, a new vector is allocated.
 * @return Pointer to output column vector or NULL on error.
 *
 * @note Matrix columns must match vector size.
 *
 * @see dmlfw_multiply_double_row_vector_with_column_vector
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *result = dmlfw_multiply_double_matrix_with_column_vector(matrix, col_vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error multiplying matrix and vector: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use result...
 * dmlfw_column_vec_double_destroy(result);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_multiply_double_matrix_with_column_vector(
    dmlfw_mat_double *left_matrix,
    dmlfw_column_vec_double *right_vector,
    dmlfw_column_vec_double *product_vector);

/**
 * @brief Subtracts two column vectors element-wise.
 *
 * @param left_vector [in] Minuend vector (non-NULL).
 * @param right_vector [in] Subtrahend vector (non-NULL).
 * @param difference_vector [in,out] Optional output vector matching input size.
 *                                  If NULL, a new vector is allocated.
 * @return Pointer to difference vector or NULL on error.
 *
 * @note Vectors must have the same size.
 *
 * @see dmlfw_add_double_column_vector
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *diff = dmlfw_subtract_double_column_vector(vec1, vec2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error subtracting vectors: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use diff...
 * dmlfw_column_vec_double_destroy(diff);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_subtract_double_column_vector(
    dmlfw_column_vec_double *left_vector,
    dmlfw_column_vec_double *right_vector,
    dmlfw_column_vec_double *difference_vector);

/**
 * @brief Multiplies a scalar with a column vector.
 *
 * @param scalar_value [in] Scalar to multiply.
 * @param vector [in] Input vector (non-NULL).
 * @param product_vector [in,out] Optional output vector matching input size.
 *                              If NULL, a new vector is allocated.
 * @return Pointer to product vector or NULL on error.
 *
 * @see dmlfw_multiply_double_row_vector_with_column_vector
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *prod = dmlfw_multiply_double_scalar_with_column_vector(3.14, vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error scaling vector: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use prod...
 * dmlfw_column_vec_double_destroy(prod);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_multiply_double_scalar_with_column_vector(
    double scalar_value,
    dmlfw_column_vec_double *vector,
    dmlfw_column_vec_double *product_vector);

/**
 * @brief Multiplies two matrices.
 *
 * @param left_matrix [in] Left matrix (non-NULL).
 * @param right_matrix [in] Right matrix (non-NULL).
 * @param product_matrix [in,out] Optional output matrix with appropriate dimensions.
 *                              If NULL, a new matrix is allocated.
 * @return Pointer to product matrix or NULL on error.
 *
 * @note Left matrix columns must match right matrix rows.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *prod = dmlfw_multiply_double_matrix_with_matrix(mat1, mat2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error multiplying matrices: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use prod
 * dmlfw_mat_double_destroy(prod);
 * @endcode
 */
dmlfw_mat_double * dmlfw_multiply_double_matrix_with_matrix(
    dmlfw_mat_double *left_matrix,
    dmlfw_mat_double *right_matrix,
    dmlfw_mat_double *product_matrix);

/**
 * @brief Adds two matrices element-wise.
 *
 * @param left_matrix [in] First matrix (non-NULL).
 * @param right_matrix [in] Second matrix (non-NULL).
 * @param new_matrix [in,out] Optional output matrix, must match input sizes.
 *                          If NULL, new matrix allocated.
 * @return Pointer to sum matrix or NULL on error.
 *
 * @note Matrices must be of equal dimensions.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *sum = dmlfw_add_double_matrix_with_matrix(mat1, mat2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error adding matrices: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use sum
 * dmlfw_mat_double_destroy(sum);
 * @endcode
 */
dmlfw_mat_double * dmlfw_add_double_matrix_with_matrix(
    dmlfw_mat_double *left_matrix,
    dmlfw_mat_double *right_matrix,
    dmlfw_mat_double *new_matrix);

/**
 * @brief Subtracts second matrix from first element-wise.
 *
 * @param left_matrix [in] Minuend matrix (non-NULL).
 * @param right_matrix [in] Subtrahend matrix (non-NULL).
 * @param new_matrix [in,out] Optional output matrix, must match input sizes.
 *                          If NULL, new matrix allocated.
 * @return Pointer to difference matrix or NULL on error.
 *
 * @note Matrices must be of equal dimensions.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *diff = dmlfw_subtract_double_matrix_from_matrix(mat1, mat2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error subtracting matrices: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use diff
 * dmlfw_mat_double_destroy(diff);
 * @endcode
 */
dmlfw_mat_double * dmlfw_subtract_double_matrix_from_matrix(
    dmlfw_mat_double *left_matrix,
    dmlfw_mat_double *right_matrix,
    dmlfw_mat_double *new_matrix);

/**
 * @brief Multiplies two square matrices using Strassen's algorithm.
 *
 * @param left_matrix [in] Left square matrix with power-of-two dimensions (non-NULL).
 * @param right_matrix [in] Right square matrix matching left matrix dimensions (non-NULL).
 * @param new_matrix [in,out] Optional output matrix, square of same size.
 *                          If NULL, new matrix allocated.
 * @return Pointer to product matrix or NULL on error.
 *
 * @note Dimensions must be power-of-two for Strassen's algorithm.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *prod = dmlfw_multiply_double_matrix_with_matrix_strassens(mat1, mat2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error multiplying matrices with Strassen's: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use prod
 * dmlfw_mat_double_destroy(prod);
 * @endcode
 */
dmlfw_mat_double * dmlfw_multiply_double_matrix_with_matrix_strassens(
    dmlfw_mat_double *left_matrix,
    dmlfw_mat_double *right_matrix,
    dmlfw_mat_double *new_matrix);

/**
 * @brief Subtracts each element of a column vector from a scalar.
 *
 * @param vector [in] Input vector (non-NULL).
 * @param scalar_value [in] Scalar to subtract from.
 * @param new_vector [in,out] Optional output vector same size as input.
 *                        If NULL, new vector allocated.
 * @return Pointer to output vector or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *result = dmlfw_subtract_double_column_vector_from_scalar(vec, 3.14, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error subtracting scalar from vector: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use result...
 * dmlfw_column_vec_double_destroy(result);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_subtract_double_column_vector_from_scalar(
    dmlfw_column_vec_double *vector,
    double scalar_value,
    dmlfw_column_vec_double *new_vector);

/**
 * @brief Element-wise multiply two column vectors.
 *
 * @param left_vector [in] First vector (non-NULL).
 * @param right_vector [in] Second vector (non-NULL).
 * @param new_vector [in,out] Optional output vector, same size as inputs.
 *                        If NULL, new vector allocated.
 * @return Pointer to output vector or NULL on error.
 *
 * @note Vectors must have equal size.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *result = dmlfw_element_wise_multiply_double_column_vector(vec1, vec2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in element-wise multiplication: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use result...
 * dmlfw_column_vec_double_destroy(result);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_element_wise_multiply_double_column_vector(
    dmlfw_column_vec_double *left_vector,
    dmlfw_column_vec_double *right_vector,
    dmlfw_column_vec_double *new_vector);

/**
 * @brief Adds two column vectors element-wise.
 *
 * @param left_vector [in] First vector (non-NULL).
 * @param right_vector [in] Second vector (non-NULL).
 * @param new_vector [in,out] Optional output vector, same size.
 *                        If NULL, new vector allocated.
 * @return Pointer to output vector or NULL on error.
 *
 * @note Vectors must have equal size.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *sum = dmlfw_add_double_column_vector(vec1, vec2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error adding vectors: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use sum...
 * dmlfw_column_vec_double_destroy(sum);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_add_double_column_vector(
    dmlfw_column_vec_double *left_vector,
    dmlfw_column_vec_double *right_vector,
    dmlfw_column_vec_double *new_vector);

/** @} */ // end of group
#endif
