/**
 * @file dmlfw_mat_string.h
 * @brief String matrix types and operations.
 *
 * This header defines opaque matrix structures specialized for string data.
 * It provides APIs for creation, destruction, element access and mutation,
 * CSV import/export, matrix transpose, shuffle, copy, and column deletion.
 *
 * Error Handling:
 * ----------------
 * All functions report errors via the centralized error API.
 * Use `dmlfw_error()` to check for errors after calls.
 * `dmlfw_get_error_string()` and `dmlfw_get_debug_string()` provide error information.
 *
 * Ownership:
 * ----------------
 * Functions returning new matrix pointers transfer ownership to the caller,
 * who is responsible for freeing with `dmlfw_mat_string_destroy()`.
 *
 * Indexing Notes:
 * ---------------
 * All indices are zero-based.
 *
 * @defgroup matrix String Matrix Module
 * @ingroup matrix
 * @brief Core string matrix types and utilities.
 *
 * @example example_mat_string.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include "dmlfw_mat_string.h"
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_mat_string *mat = dmlfw_mat_string_create_new(3, 3);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to create matrix: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_mat_string_set(mat, 0, 0, "Hello");
 *     char *str = NULL;
 *     dmlfw_mat_string_get(mat, 0, 0, &str);
 *     if (!dmlfw_error() && str != NULL) {
 *         printf("Value at (0,0): %s\n", str);
 *         free(str);
 *     } else {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to get string: %s\nDebug info: %s\n", err, dbg);
 *     }
 *
 *     dmlfw_mat_string_destroy(mat);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */

#ifndef __DMLFW__MAT__STRING__
#define __DMLFW__MAT__STRING__

#include <dmlfw_types.h>

struct __dmlfw_mat_string;

/**
 * @brief Opaque structure representing a string matrix.
 *
 * The internal representation is hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_mat_string dmlfw_mat_string;

/**
 * @brief Creates a new string matrix with specified rows and columns.
 *
 * @param rows [in] Number of rows (must be > 0).
 * @param columns [in] Number of columns (must be > 0).
 * @return Pointer to new matrix or NULL on allocation failure or invalid args.
 *
 * @see dmlfw_mat_string_destroy
 *
 * @note Caller must destroy with `dmlfw_mat_string_destroy()`.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_string *mat = dmlfw_mat_string_create_new(5, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Create failed: %s\nDebug: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_mat_string *dmlfw_mat_string_create_new(dimension_t rows, dimension_t columns);

/**
 * @brief Destroys a string matrix and frees all allocated memory.
 *
 * @param matrix [in] Pointer to matrix, or NULL (no-op).
 *
 * @see dmlfw_mat_string_create_new
 *
 * Usage example:
 * @code
 * dmlfw_mat_string_destroy(mat);
 * @endcode
 */
void dmlfw_mat_string_destroy(dmlfw_mat_string *matrix);

/**
 * @brief Loads a matrix and header from a CSV file.
 *
 * @param csv_file_name [in] CSV file path (non-NULL).
 * @param matrix [in,out] Optional matrix to reuse or NULL.
 * @param header [out] Pointer to receive the newly allocated header row vector.
 * @return Pointer to loaded matrix or NULL on error.
 *
 * @see dmlfw_mat_string_to_csv 
 *
 * @note Caller must destroy returned matrix and header.
 * @note Header is always a row vector.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_string *mat = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * mat = dmlfw_mat_string_from_csv("data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Load failed: %s\nDebug: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_string_destroy(mat);
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
dmlfw_mat_string *dmlfw_mat_string_from_csv(const char *csv_file_name, dmlfw_mat_string *matrix, dmlfw_row_vec_string **header);

/**
 * @brief Saves a string matrix and header to a CSV file.
 *
 * @param matrix [in] Matrix to save (non-NULL).
 * @param csv_file_name [in] Output CSV file path (non-NULL).
 * @param header [in] Header row vector (non-NULL).
 *
 * @see dmlfw_mat_string_from_csv
 *
 * @note File will be overwritten if it exists.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_string_to_csv(mat, "out.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error exporting matrix: %s\nDebug: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_string_to_csv(dmlfw_mat_string *matrix, const char *csv_file_name, dmlfw_row_vec_string *header);

/**
 * @brief Retrieves the string at specified row and column.
 *
 * @param matrix [in] Matrix (non-NULL).
 * @param row [in] Zero-based row index.
 * @param column [in] Zero-based column index.
 * @param string [out] Allocation pointer to receive string copy (must not be NULL).
 *
 * @see dmlfw_mat_string_set
 *
 * @note Returned string must be freed by caller.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * char *str = NULL;
 * dmlfw_mat_string_get(mat, 0, 0, &str);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to retrieve value from (1,2) : %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;  
 * } else {
 *     printf("Value at (0,0): %s\n", str);
 *     free(str);
 * }
 * @endcode
 */
void dmlfw_mat_string_get(dmlfw_mat_string *matrix, index_t row, index_t column, char **string);

/**
 * @brief Sets the string at specified row and column.
 *
 * @param matrix [in,out] Matrix (non-NULL).
 * @param row [in] Zero-based row index.
 * @param column [in] Zero-based column index.
 * @param string [in] String value to copy into matrix (non-NULL).
 *
 * @see dmlfw_mat_string_get
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_string_set(mat, 0, 0, "example");
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to set value at (1,2) : %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_string_set(dmlfw_mat_string *matrix, index_t row, index_t column, char *string);

/**
 * @brief Retrieves dimensions of the string matrix.
 *
 * @param matrix [in] Matrix (non-NULL).
 * @param rows [out] Pointer to receive row count (non-NULL).
 * @param columns [out] Pointer to receive column count (non-NULL).
 *
 * @see dmlfw_mat_string_create_new
 *
 * Usage example:
 * @code
 * dimension_t rows, cols;
 * dmlfw_mat_string_get_dimensions(mat, &rows, &cols);
 * @endcode
 */
void dmlfw_mat_string_get_dimensions(dmlfw_mat_string *matrix, dimension_t *rows, dimension_t *columns);

/**
 * @brief Transposes a string matrix.
 *
 * @param matrix [in] Matrix to transpose (non-NULL).
 * @param transposed_matrix [in,out] Optional matrix container to receive transpose, or NULL.
 * @return Pointer to transposed matrix or NULL on error.
 *
 * Usage example:
 * @code
 * dmlfw_mat_string *transposed = dmlfw_mat_string_transpose(mat, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_string_destroy(transposed);
 * @endcode
 */
dmlfw_mat_string *dmlfw_mat_string_transpose(dmlfw_mat_string *matrix, dmlfw_mat_string *transposed_matrix);

/**
 * @brief Copies a submatrix region from source to target matrix.
 *
 * @param target [in,out] Destination matrix (non-NULL).
 * @param source [in] Source matrix (non-NULL).
 * @param target_row_index [in] Start row index in target.
 * @param target_column_index [in] Start column index in target.
 * @param source_from_row_index [in] Starting row index in source.
 * @param source_from_column_index [in] Starting column index in source.
 * @param source_to_row_index [in] Ending row index in source.
 * @param source_to_column_index [in] Ending column index in source.
 *
 * @see dmlfw_mat_string_transpose
 *
 * Usage example:
 * @code
 * dmlfw_mat_string_copy(dest, src, 0, 0, 1, 1, 3, 3);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error copying matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_string_copy(dmlfw_mat_string *target, dmlfw_mat_string *source,
                           index_t target_row_index, index_t target_column_index,
                           index_t source_from_row_index, index_t source_from_column_index,
                           index_t source_to_row_index, index_t source_to_column_index);

/**
 * @brief Shuffles rows of the string matrix.
 *
 * @param matrix [in] Matrix to shuffle (non-NULL).
 * @param how_many_times_to_shuffle [in] Number of shuffle iterations.
 * @param shuffled_matrix [in,out] Optional matrix container for result, or NULL.
 * @return Pointer to shuffled matrix or NULL on error.
 *
 * @see dmlfw_mat_string_clone
 *
 * Usage example:
 * @code
 * dmlfw_mat_string *shuffled = dmlfw_mat_string_shuffle(mat, 10, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error shuffling matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_string_destroy(shuffled);
 * @endcode
 */
dmlfw_mat_string *dmlfw_mat_string_shuffle(dmlfw_mat_string *matrix, uint8_t how_many_times_to_shuffle, dmlfw_mat_string *shuffled_matrix);

/**
 * @brief Deletes specified columns from the matrix.
 *
 * @param matrix [in] Source matrix (non-NULL).
 * @param indexes [in] Array of zero-based column indexes to delete.
 * @param indexes_size [in] Number of indexes to delete.
 * @param new_matrix [in,out] Optional matrix container for result, or NULL.
 * @return Pointer to new matrix after deletion, or NULL on error.
 *
 * @see dmlfw_mat_string_clone
 *
 * Usage example:
 * @code
 * index_t cols_to_delete[] = {1,3};
 * dmlfw_mat_string *reduced = dmlfw_mat_string_delete_columns(mat, cols_to_delete, 2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to delete column at index 2 from matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_string_destroy(reduced);
 * @endcode
 */
dmlfw_mat_string *dmlfw_mat_string_delete_columns(dmlfw_mat_string *matrix, index_t *indexes, dimension_t indexes_size, dmlfw_mat_string *new_matrix);

#endif // __DMLFW__MAT__STRING__

/** @} */
