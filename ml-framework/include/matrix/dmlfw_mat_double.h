/**
 * @file dmlfw_mat_double.h
 * @brief Double precision matrix types and operations.
 *
 * This header defines opaque matrix structures for double precision floating-point data.
 * It provides APIs for creation, destruction, element access and mutation, CSV import/export,
 * matrix operations such as transpose, reshape, shuffle, block extraction, and statistical functions.
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
 * who is responsible for freeing with `dmlfw_mat_double_destroy()`.
 *
 * Indexing Notes:
 * ---------------
 * - All indices are zero-based, except for functions like `dmlfw_mat_double_get_block_from_csv()`
 *   which may use one-based indexing (documented explicitly in their parameter list).
 * - Range parameters (start/end) are inclusive unless explicitly stated otherwise.
 *
 * @defgroup matrix Double Precision Matrix Module
 * @ingroup matrix
 * @brief Core double precision matrix types and utilities.
 *
 * @example example_mat_double.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_mat_double.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_mat_double *mat = dmlfw_mat_double_create_new(3, 3);
 *     double val=0.0;
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to create matrix: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *     dmlfw_mat_double_set(mat, 0, 0, 1.23);
 *     val = dmlfw_mat_double_get(mat, 0, 0);
 *     printf("Value at (0,0): %lf\n", val);
 *
 *     dmlfw_mat_double_destroy(mat);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */

#ifndef DMLFW_MAT_DOUBLE_H
#define DMLFW_MAT_DOUBLE_H

#include <dmlfw_types.h>

struct __dmlfw_mat_double;

/**
 * @brief Opaque structure representing a double precision matrix.
 *
 * The internal representation is hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_mat_double dmlfw_mat_double;


/**
 * @brief Creates a new matrix with specified rows and columns.
 *
 * @param rows [in] Number of rows (must be > 0).
 * @param columns [in] Number of columns (must be > 0).
 * @return Pointer to new matrix, or NULL if allocation fails or invalid args.
 *
 * @see dmlfw_mat_double_destroy
 *
 * @note Caller must free with `dmlfw_mat_double_destroy()`.
 * @note Check errors with the centralized error API.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *mat = dmlfw_mat_double_create_new(5, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Create failed: %s\nDebug: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_create_new(dimension_t rows, dimension_t columns);


/**
 * @brief Destroys a matrix and frees all associated memory.
 *
 * @param matrix [in] Pointer to matrix, or NULL (no-op).
 *
 * @see dmlfw_mat_double_create_new
 *
 * Usage example:
 * @code
 * dmlfw_mat_double_destroy(mat);
 * @endcode
 */
void dmlfw_mat_double_destroy(dmlfw_mat_double *matrix);


/**
 * @brief Loads a matrix and header from a CSV file.
 *
 * @param csv_file_name [in] CSV file path (non-NULL).
 * @param matrix [in,out] Optional matrix to reuse; if NULL a new matrix is allocated.
 * @param header [out] Pointer to receive newly allocated header (non-NULL).
 * @return Pointer to loaded matrix, or NULL on error.
 *
 * @note Caller must free returned matrix and header.
 * @note Header is always a row vector.
 * @note Check errors with the centralized error API.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *mat = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * mat = dmlfw_mat_double_from_csv("data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Load failed: %s\nDebug: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(mat);
 * dmlfw_vec_string_destroy(header);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_from_csv(const char *csv_file_name,
                                           dmlfw_mat_double *matrix,
                                           dmlfw_row_vec_string **header);


/**
 * @brief Saves a matrix and its header to a CSV file.
 *
 * @param matrix [in] Matrix to save (must not be NULL).
 * @param csv_file_name [in] Output CSV file path (must not be NULL).
 * @param header [in] Row vector header (must not be NULL).
 *
 *  @see dmlfw_mat_double_from_csv
 *
 * @note File is overwritten if it exists.
 * @note Header row vector size must exactly match the number of matrix columns.
 * @note Check errors with the centralized error API after the call.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_to_csv(mat, "output.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error exporting matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_to_csv(dmlfw_mat_double *matrix,
                             const char *csv_file_name,
                             dmlfw_row_vec_string *header);


/**
 * @brief Returns value at a specific matrix position.
 *
 * @param matrix [in] Matrix (non-NULL).
 * @param row [in] row index.
 * @param column [in] column index.
 * @return Value at position or 0.0 if error.
 *
 * @see dmlfw_mat_double_set
 *
 * @note Checks bounds and errors.
 *
 * Usage example:
 * @code
 * double val = dmlfw_mat_double_get(mat, 1, 2);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to retrieve value from (1,2) : %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;  
 * }
 * printf("Value: %lf\n", val);
 * @endcode
 */
double dmlfw_mat_double_get(dmlfw_mat_double *matrix, index_t row, index_t column);


/**
 * @brief Sets value at a specific matrix position.
 *
 * @param matrix [in,out] Matrix (non-NULL).
 * @param row [in] row index.
 * @param column [in] column index.
 * @param value [in] Value to set.
 *
 * @see dmlfw_mat_double_get
 *
 * @note Checks bounds and errors.
 *
 * Usage example:
 * @code
 * dmlfw_mat_double_set(mat, 1, 2, 3.14);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to set value at (1,2) : %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_set(dmlfw_mat_double *matrix, index_t row, index_t column, double value);


/**
 * @brief Retrieves the matrix dimensions.
 *
 * @param matrix [in] Matrix (non-NULL).
 * @param rows [out] Pointer to store row count (non-NULL).
 * @param columns [out] Pointer to store column count (non-NULL).
 *
 * @see dmlfw_mat_double_create_new
 *
 * Usage example:
 * @code
 * dimension_t rows, cols;
 * dmlfw_mat_double_get_dimensions(mat, &rows, &cols);
 * @endcode
 */
void dmlfw_mat_double_get_dimensions(dmlfw_mat_double *matrix,
                                     dimension_t *rows,
                                     dimension_t *columns);


/**
 * @brief Copies a submatrix from source to target at specified offsets.
 *
 * @param target [in,out] Destination matrix (non-NULL).
 * @param source [in] Source matrix (non-NULL). 
 * @param target_row_index [in] row index to start copy at in target  
 * @param target_column_index [in] column index to start copy at in target  
 * @param source_from_row_index [in] start row index in source  
 * @param source_from_column_index [in] start column index in source  
 * @param source_to_row_index [in] end row index in source  
 * @param source_to_column_index [in] end column index in source  
 *
 * @see dmlfw_mat_double_clone
 *
 * Usage example:
 * @code
 * dmlfw_mat_double_copy(dest, src, 0, 0, 1, 1, 3, 3);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error copying matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_copy(dmlfw_mat_double *target,
                           dmlfw_mat_double *source,
                           index_t target_row_index,
                           index_t target_column_index,
                           index_t source_from_row_index,
                           index_t source_from_column_index,
                           index_t source_to_row_index,
                           index_t source_to_column_index);

/**
 * @brief Fills a subregion of the matrix with the specified value.
 *
 * @param matrix [in,out] Matrix to fill (must not be NULL).
 * @param from_row_index [in] starting row index of the subregion
 * @param from_column_index [in] starting column index of the subregion
 * @param to_row_index [in] ending row index of the subregion  
 * @param to_column_index [in] ending column index of the subregion  
 * @param value [in] The value to fill in the specified subregion
 *
 * @see dmlfw_mat_double_get
 * @see dmlfw_mat_double_set
 *
 * @note Indices are clamped if out-of-bounds.
 * @note Error status is set if ranges are invalid or matrix is NULL.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_fill(mat, 1, 1, 3, 3, 5.5);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error filling matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_fill(dmlfw_mat_double *matrix,
                           index_t from_row_index,
                           index_t from_column_index,
                           index_t to_row_index,
                           index_t to_column_index,
                           double value);


/**
 * @brief Creates or fills a column vector from the specified column of the matrix.
 *
 * @param matrix [in] Matrix (must not be NULL).
 * @param column_index [in] index of the column to extract.
 * @param vector [in,out] Optional existing column vector to reuse, or NULL.
 * @return Pointer to the column vector containing the specified column,
 *         or NULL if an error occurs.
 *
 * @see dmlfw_mat_double_create_new
 * 
 * @note If vector is NULL, a new one is allocated and ownership is transferred to caller.
 * @note Check errors with the centralized error API after the call.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *col_vec = dmlfw_mat_double_create_column_vec(mat, 2, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_column_vec_double_destroy(col_vec);
 * @endcode
 */
dmlfw_column_vec_double *dmlfw_mat_double_create_column_vec(dmlfw_mat_double *matrix,
                                                           index_t column_index,
                                                           dmlfw_column_vec_double *vector);


/**
 * @brief Returns a shuffled version of the matrix, shuffling rows specified times.
 *
 * @param matrix [in] Source matrix (must not be NULL).
 * @param how_many_times_to_shuffle [in] Number of shuffle iterations (non-zero).
 * @param shuffled_matrix [in,out] Optional matrix to receive shuffled output, or NULL.
 * @return Pointer to matrix containing shuffled rows, or NULL if an error occurs.
 *
 * @see dmlfw_mat_double_clone
 *
 * @note If shuffled_matrix is NULL, a new matrix is allocated and ownership is transferred.
 * @note Check errors with the centralized error API after the call.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *shuffled = dmlfw_mat_double_shuffle(mat, 10, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error shuffling matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(shuffled);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_shuffle(dmlfw_mat_double *matrix,
                                           uint8_t how_many_times_to_shuffle,
                                           dmlfw_mat_double *shuffled_matrix);



/**
 * @brief Transposes a matrix.
 *
 * @param matrix [in] Matrix to transpose (must not be NULL).
 * @param transposed_matrix [in,out] Optional matrix to receive transpose, or NULL.
 * @return Pointer to the transposed matrix or NULL on error.
 *
 * @see dmlfw_mat_double_clone 
 *
 * @note Caller owns newly created matrices.
 * @note Check errors with the centralized error API after the call.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *transposed = dmlfw_mat_double_transpose(mat, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(transposed);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_transpose(dmlfw_mat_double *matrix,
                                             dmlfw_mat_double *transposed_matrix);


/**
 * @brief Finds the minimum value in a specified submatrix region.
 *
 * @param matrix [in] Matrix (must not be NULL).
 * @param start_row_index [in] starting row  
 * @param start_column_index [in] starting column  
 * @param end_row_index [in] ending row  
 * @param end_column_index [in] ending column  
 * @return Minimum value or 0.0 if an error occurred.
 *
 * * @see dmlfw_mat_double_get_maximum
 * @note Indices are clamped.
 * @note Check errors with the centralized error API after the call.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double min_val = dmlfw_mat_double_get_minimum(mat, 0, 0, 4, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error finding minimum: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
double dmlfw_mat_double_get_minimum(dmlfw_mat_double *matrix,
                                    index_t start_row_index,
                                    index_t start_column_index,
                                    index_t end_row_index,
                                    index_t end_column_index);


/**
 * @brief Finds the maximum value in a specified submatrix region.
 *
 * @param matrix [in] Matrix (must not be NULL).
 * @param start_row_index [in] starting row  
 * @param start_column_index [in] starting column  
 * @param end_row_index [in] ending row  
 * @param end_column_index [in] ending column  
 * @return Maximum value in the submatrix or 0.0 if error occurred.
 *
 * @note Indices are clamped to valid ranges.
 * @note Check errors using the centralized error API.
 *
 * * @see dmlfw_mat_double_get_minimum
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double max_val = dmlfw_mat_double_get_maximum(mat, 0, 0, 4, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error finding maximum: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * else {
 *     printf("Maximum value: %lf\n", max_val);
 * }
 * @endcode
 */
double dmlfw_mat_double_get_maximum(dmlfw_mat_double *matrix,
                                    index_t start_row_index,
                                    index_t start_column_index,
                                    index_t end_row_index,
                                    index_t end_column_index);


/**
 * @brief Computes the mean of elements in a specified submatrix region.
 *
 * @param matrix [in] Matrix (must not be NULL).
 * @param start_row_index [in] starting row  
 * @param start_column_index [in] starting column  
 * @param end_row_index [in] ending row  
 * @param end_column_index [in] ending column  
 * @return Mean value or 0.0 if an error occurred.
 *
 * @see dmlfw_mat_double_get_standard_deviation
 *
 * @note Indices are clamped to valid ranges.
 * @note Check errors using the centralized error API.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double mean_val = dmlfw_mat_double_get_mean(mat, 0, 0, 4, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error computing mean: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * else {
 *     printf("Mean value: %lf\n", mean_val);
 * }
 * @endcode
 */
double dmlfw_mat_double_get_mean(dmlfw_mat_double *matrix,
                                 index_t start_row_index,
                                 index_t start_column_index,
                                 index_t end_row_index,
                                 index_t end_column_index);


/**
 * @brief Calculates the standard deviation of elements in a specified submatrix region.
 *
 * @param matrix [in] Matrix (must not be NULL).
 * @param start_row_index [in] starting row  
 * @param start_column_index [in] starting column  
 * @param end_row_index [in] ending row  
 * @param end_column_index [in] ending column  
 * @return Standard deviation or 0.0 if an error occurred.
 *
 * @see dmlfw_mat_double_get_mean
 *
 * @note Indices are clamped.
 * @note Check errors using the centralized error API.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double std_dev = dmlfw_mat_double_get_standard_deviation(mat, 0, 0, 4, 4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error computing standard deviation: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * else {
 *     printf("Standard deviation: %lf\n", std_dev);
 * }
 * @endcode
 */
double dmlfw_mat_double_get_standard_deviation(dmlfw_mat_double *matrix,
                                               index_t start_row_index,
                                               index_t start_column_index,
                                               index_t end_row_index,
                                               index_t end_column_index);


/**
 * @brief Splits data from a CSV file into training and testing matrices.
 *
 * @param csv_file_name [in] Path to CSV file (must not be NULL).
 * @param training_data_matrix [out] Pointer to hold training data matrix.
 * @param testing_data_matrix [out] Pointer to hold testing data matrix.
 * @param testing_data_percentage [in] Percentage (0-100) of data for testing.
 *
 * @see dmlfw_mat_double_from_csv
 *
 * @note Caller is responsible for destroying returned matrices.
 * @note Check errors using the centralized error API.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *train = NULL;
 * dmlfw_mat_double *test = NULL;
 * dmlfw_mat_double_get_training_testing_data("dataset.csv", &train, &test, 20);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error splitting dataset: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * else {
 *     // use train and test...
 * }
 * dmlfw_mat_double_destroy(train);
 * dmlfw_mat_double_destroy(test);
 * @endcode
 */
void dmlfw_mat_double_get_training_testing_data(const char *csv_file_name,
                                                dmlfw_mat_double **training_data_matrix,
                                                dmlfw_mat_double **testing_data_matrix,
                                                uint8_t testing_data_percentage);


/**
 * @brief Reshapes a matrix to new row and column counts.
 *
 * @param matrix_to_reshape [in,out] Pointer to matrix pointer to reshape.
 * @param new_rows_count [in] New number of rows (must be > 0).
 * @param new_columns_count [in] New number of columns (must be > 0).
 *
 * @see dmlfw_mat_double_clone
 *
 * @note Original matrix is destroyed and replaced on success.
 * @note Sets error and NULLs pointer if failure occurs.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_reshape(&mat, 10, 10);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error reshaping matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_reshape(dmlfw_mat_double **matrix_to_reshape,
                              dimension_t new_rows_count,
                              dimension_t new_columns_count);


/**
 * @brief Right shifts matrix elements by specified number of places.
 *
 * @param matrix [in,out] Matrix to shift (must not be NULL).
 * @param how_many_places_to_shift [in] Number of positions to shift right (must be > 0).
 *
 * @see dmlfw_mat_double_left_shift
 *
 * @note Vacated positions are set to zero.
 * @note Error is set if invalid parameters.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_right_shift(mat, 2);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in right shift: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_right_shift(dmlfw_mat_double *matrix, dimension_t how_many_places_to_shift);

/**
 * @brief Left shifts matrix elements by specified number of places.
 *
 * @param matrix [in,out] Matrix to shift (must not be NULL).
 * @param how_many_places_to_shift [in] Number of positions to shift left (must be > 0).
 *
 * @see dmlfw_mat_double_right_shift
 *
 * @note Vacated positions are set to zero.
 * @note Error is set if invalid parameters.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_left_shift(mat, 2);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in left shift: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_left_shift(dmlfw_mat_double *matrix, dimension_t how_many_places_to_shift);

/**
 * @brief Creates an identity matrix of specified size.
 *
 * @param rows [in] Number of rows and columns (square matrix).
 * @return Pointer to identity matrix, or NULL on error.
 *
 * @see dmlfw_mat_double_destroy
 *
 * @note Caller must free the matrix with dmlfw_mat_double_destroy().
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *identity = dmlfw_mat_double_create_identity_matrix(4);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating identity matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(identity);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_create_identity_matrix(dimension_t rows);

/**
 * @brief Clones a matrix into a new or existing matrix container.
 *
 * @param matrix_to_clone [in] Source matrix (must not be NULL).
 * @param new_matrix [in,out] Optional existing matrix to reuse, or NULL.
 * @return Pointer to cloned matrix, or NULL on error.
 *
 * @see dmlfw_mat_double_destroy
 *
 * @note Caller must free returned matrix if newly allocated.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *clone = dmlfw_mat_double_clone(mat, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error cloning matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(clone);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_clone(dmlfw_mat_double *matrix_to_clone, dmlfw_mat_double *new_matrix);

/**
 * @brief Computes the inverse of a square matrix using Gaussian elimination.
 *
 * @param matrix_to_inverse [in] Matrix to invert (must not be NULL, square).
 * @param new_matrix [in,out] Optional container to store inverse, or NULL to allocate.
 * @return Pointer to inverse matrix, or NULL if error.
 *
 * @see dmlfw_mat_double_create_new
 * @see dmlfw_mat_double_clone
 *
 * @note Caller must free returned matrix.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *inverse = dmlfw_mat_double_inverse(mat, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error computing inverse: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(inverse);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_inverse(dmlfw_mat_double *matrix_to_inverse, dmlfw_mat_double *new_matrix);

/**
 * @brief Creates a matrix filled with random values within [min, max].
 *
 * @param rows [in] Number of rows (must be > 0).
 * @param columns [in] Number of columns (must be > 0).
 * @param min [in] Minimum random value.
 * @param max [in] Maximum random value.
 * @param new_matrix [in,out] Optional existing matrix to reuse, or NULL.
 * @return Pointer to created matrix, or NULL on error.
 * @see dmlfw_mat_double_destroy
 *
 * @note Caller must free returned matrix.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *rand_mat = dmlfw_mat_double_create_new_random_filled(5, 5, 0.0, 1.0, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating random matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(rand_mat);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_create_new_random_filled(dimension_t rows, dimension_t columns,
                                                            double min, double max,
                                                            dmlfw_mat_double *new_matrix);

/**
 * @brief Truncates the specified matrix to a subregion.
 *
 * @param matrix [in,out] Pointer to matrix pointer to modify.
 * @param from_row_index [in] starting row index
 * @param from_column_index [in] starting column index
 * @param to_row_index [in] ending row index  
 * @param to_column_index [in] ending column index  
 *
 * @see dmlfw_mat_double_clone
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_truncate(&mat, 0, 0, 3, 3);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error truncating matrix: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_truncate(dmlfw_mat_double **matrix,
                               index_t from_row_index,
                               index_t from_column_index,
                               index_t to_row_index,
                               index_t to_column_index);

/**
 * @brief Inserts columns into a matrix at a specified index.
 *
 * @param matrix [in,out] Pointer to matrix pointer to modify.
 * @param at_index [in] column index at which to insert
 * @param number_of_columns [in] Number of columns to insert
 *
 * @see dmlfw_mat_double_clone
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double_insert_columns(&mat, 2, 3);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error inserting columns: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_mat_double_insert_columns(dmlfw_mat_double **matrix,
                                     index_t at_index,
                                     dimension_t number_of_columns);

/**
 * @brief Loads a block of rows from a CSV file into a matrix.
 *
 * @param csv_file_name [in] Path to the CSV file (non-null).
 * @param matrix [in,out] Optional matrix to reuse, or NULL.
 * @param from_row [in] Start row (1-based, first row is 1)
 * @param how_many_rows [in] Number of rows to load.
 * @param pos [in,out] Optional file position pointer for resuming read, pass NULL if not used.
 * @return Pointer to loaded matrix, or NULL on error.
 *
 * @see dmlfw_mat_double_from_csv
 *
 * @note Caller frees returned matrix.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *block = dmlfw_mat_double_get_block_from_csv("data.csv", NULL, 1, 10, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error loading CSV block: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_mat_double_destroy(block);
 * @endcode
 */
dmlfw_mat_double *dmlfw_mat_double_get_block_from_csv(const char *csv_file_name,
                                                      dmlfw_mat_double *matrix,
                                                      uint64_t from_row,
                                                      uint64_t how_many_rows,
                                                      long *pos);


#endif /* DMLFW_MAT_DOUBLE_H */

/** @} */
