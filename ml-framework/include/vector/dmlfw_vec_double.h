#ifndef __DMLFW__VEC__DOUBLE__
#define __DMLFW__VEC__DOUBLE__
/**
 * @file dmlfw_vec_double.h
 * @brief Double precision column and row vector types and functions.
 * @ingroup vector_double
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This header defines opaque vector data structures for double precision
 * floating-point values. It provides APIs for creating, destroying,
 * accessing, modifying, and manipulating both column and row vectors.
 *
 * Common operations include:
 * - Creation and destruction
 * - Element access and mutation
 * - Transposing between row and column vectors
 * - CSV import/export
 * - Basic statistics (mean, size, etc.)
 * - Copying contents
 *
 * Error Handling:
 * ----------------
 * All functions report errors through the centralized error API:
 * - Call @ref dmlfw_error() to check whether an error occurred.
 * - Use @ref dmlfw_get_error_string() and @ref dmlfw_get_debug_string()
 *   for error details and debugging info.
 *
 * Unless otherwise stated:
 * - Passing NULL pointers or invalid arguments sets error status.
 * - On error, functions return NULL, 0, or 0.0 depending on return type.
 *
 *
 * @example example_dmlfw_vec_double.c
 * Typical usage example:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_vector.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     // Create a column vector with 3 rows (elements)
 *     dmlfw_column_vec_double *col_vec = dmlfw_column_vec_double_create_new(3);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error creating column vector: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_column_vec_double_set(col_vec, 0, 1.0);
 *     dmlfw_column_vec_double_set(col_vec, 1, 2.0);
 *     dmlfw_column_vec_double_set(col_vec, 2, 3.0);
 *
 *     double mean = dmlfw_column_vec_double_get_mean(col_vec);
 *     printf("Mean of column vector: %lf\n", mean);
 *
 *     dmlfw_column_vec_double_destroy(col_vec);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 *
 */


#include <dmlfw_types.h>

struct __dmlfw_column_vec_double;
struct __dmlfw_row_vec_double;

/**
 * @ingroup vector
 * @brief Opaque structure representing a column vector of doubles.
 *
 * The internal details are hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_column_vec_double dmlfw_column_vec_double;

/**
 * @ingroup vector
 * @brief Opaque structure representing a row vector of doubles.
 *
 * The internal details are hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_row_vec_double dmlfw_row_vec_double;


//
// Column vector API
//

/**
 * @brief Creates a new column vector of specified size.
 *
 * @param size [in] Number of elements (must be > 0).
 * @return Pointer to new vector, or NULL if an error occurred.
 *
 * @note Caller must free with dmlfw_column_vec_double_destroy().
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *vec = dmlfw_column_vec_double_create_new(10);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_column_vec_double *
dmlfw_column_vec_double_create_new(dimension_t rows);

/**
 * @brief Destroys a column vector and frees its memory.
 *
 * @param vector [in] Pointer to the column vector, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_column_vec_double_destroy(vec);
 * @endcode
 */
void dmlfw_column_vec_double_destroy(dmlfw_column_vec_double *vector);

/**
 * @brief Retrieves the value of an element in a column vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param index [in] Zero-based index of the element.
 * @return Element value, or 0.0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double val = dmlfw_column_vec_double_get(vec, 1);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error getting value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
double
dmlfw_column_vec_double_get(dmlfw_column_vec_double *vector,
                            index_t index);

/**
 * @brief Sets the value of an element in a column vector.
 *
 * @param vector [in,out] Column vector (must not be NULL).
 * @param index [in] Zero-based index of the element.
 * @param value [in] Value to set.
 *
 * @note If index is out of bounds, error status is set.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double_set(vec, 2, 5.5);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error setting value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_column_vec_double_set(dmlfw_column_vec_double *vector,
                                 index_t index,
                                 double value);

/**
 * @brief Creates or fills a column vector with a specified value.
 *
 * - If `vector` is NULL → allocates new vector of size `size` and fills with `value`.
 * - If `vector` exists and matches `size` → fills with `value`.
 * - If size mismatches → sets error and returns NULL.
 *
 * @param size [in] Number of elements (must be > 0).
 * @param value [in] Value to assign to all elements.
 * @param vector [in,out] Optional existing vector to reuse, or NULL.
 * @return Pointer to valid vector on success, or NULL if an error occurred.
 *
 * @note Caller must free newly allocated vectors.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *vec = dmlfw_column_vec_double_create_new_filled(4, 3.14, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating filled vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_column_vec_double *
dmlfw_column_vec_double_create_new_filled(dimension_t rows,
                                          double value,
                                          dmlfw_column_vec_double *vector);

/**
 * @brief Transposes a column vector into a row vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param transposed_vector [in,out] Optional existing row vector to reuse, or NULL.
 * @return Pointer to row vector, or NULL if an error occurred.
 *
 * @note Caller must free newly allocated row vectors.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double *row_vec = dmlfw_column_vec_double_transpose(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing to row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 *
 * @see dmlfw_row_vec_double_transpose
 */
dmlfw_row_vec_double *
dmlfw_column_vec_double_transpose(dmlfw_column_vec_double *vector,
                                  dmlfw_row_vec_double *transposed_vector);

/**
 * @brief Returns the number of elements in a column vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @return Number of elements, or 0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 */
dimension_t
dmlfw_column_vec_double_get_size(dmlfw_column_vec_double *vector);

/**
 * @brief Computes the mean of all elements in a column vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @return Mean value, or 0.0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 */
double
dmlfw_column_vec_double_get_mean(dmlfw_column_vec_double *vector);

/**
 * @brief Exports a column vector to a CSV file with a header.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param csv_file_name [in] Output CSV file path (must not be NULL).
 * @param header [in] Single-element row vector of strings (must not be NULL).
 *
 * Assumptions:
 * - Header must have exactly one string element.
 * - File is overwritten if it exists.
 *
 * Error Cases:
 * - Invalid parameters or file operation failure → sets error status, does nothing.
 *
 * @note The file will be overwritten if it exists.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * @see dmlfw_row_vec_string
 * @see dmlfw_vec_string.h
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *header = dmlfw_row_vec_string_create_new(1);
 * dmlfw_row_vec_string_set(header, 0, "values");
 * dmlfw_column_vec_double_to_csv(vec, "data.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error exporting to CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
void
dmlfw_column_vec_double_to_csv(dmlfw_column_vec_double *vector,
                              const char *csv_file_name,
                              dmlfw_row_vec_string *header);

/**
 * @brief Loads a column vector from a one-column CSV file.
 *
 * Expects a CSV file where:
 * - First row is a single header string.
 * - Each subsequent row contains one numeric value.
 *
 * @param csv_file_name [in] CSV file path (must not be NULL).
 * @param vector [in,out] Optional existing vector to reuse, or NULL.
 * @param header [out] New single-element row vector containing header string (must not be NULL).
 * @return Pointer to column vector, or NULL if an error occurred.
 *
 * @note Caller must destroy both the returned vector and header. 
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note If `vector` is provided, its size must match the number of data rows.
 *
 * Error Cases:
 * - Invalid parameters, invalid CSV format, or file operation failure → sets error status and returns NULL.
 *
 * @note Check errors using the centralized error API after calling this function.
 * @see dmlfw_row_vec_string
 * @see dmlfw_vec_string.h
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *vec = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * vec = dmlfw_column_vec_double_from_csv("data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error loading from CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_column_vec_double_destroy(vec);
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
dmlfw_column_vec_double *
dmlfw_column_vec_double_from_csv(const char *csv_file_name,
                                dmlfw_column_vec_double *vector,
                                dmlfw_row_vec_string **header);


/**
 * @ingroup vector
 * @brief Copies the contents of one column vector to another.
 *
 * Assumptions:
 * - Both vectors must be non-NULL and have the same size.
 *
 * Error Cases:
 * - NULL target or source vectors.
 * - Size mismatch between vectors.
 *
 * @param target_vector [in,out] Destination column vector (must not be NULL).
 * @param source_vector [in] Source column vector (must not be NULL).
 *
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double_copy(target, source);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error copying column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void
dmlfw_column_vec_double_copy(dmlfw_column_vec_double *target_vector,
                            dmlfw_column_vec_double *source_vector);



//
// Row vector API
//

/**
 * @ingroup vector
 * @brief Creates a new row vector of specified size.
 *
 * @param size [in] Number of elements (must be > 0).
 * @return Pointer to newly allocated row vector, or NULL if an error occurred.
 *
 * @note Caller must free with dmlfw_row_vec_double_destroy().
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double *vec = dmlfw_row_vec_double_create_new(12);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_row_vec_double *
dmlfw_row_vec_double_create_new(dimension_t columns);

/**
 * @ingroup vector
 * @brief Destroys a row vector and frees its memory.
 *
 * @param vector [in] Pointer to the row vector, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_row_vec_double_destroy(vec);
 * @endcode
 */
void
dmlfw_row_vec_double_destroy(dmlfw_row_vec_double *vector);

/**
 * @ingroup vector
 * @brief Retrieves the value of an element in a row vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param index [in] Zero-based index of the element.
 * @return Element value, or 0.0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * double val = dmlfw_row_vec_double_get(vec, 5);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error getting value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
double
dmlfw_row_vec_double_get(dmlfw_row_vec_double *vector, index_t index);

/**
 * @ingroup vector
 * @brief Sets the value of an element in a row vector.
 *
 * @param vector [in,out] Row vector (must not be NULL).
 * @param index [in] Zero-based index of the element.
 * @param value [in] Value to set.
 *
 * @note If index is out of bounds, error status is set.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double_set(vec, 3, 7.7);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error setting value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void
dmlfw_row_vec_double_set(dmlfw_row_vec_double *vector, index_t index, double value);

/**
 * @ingroup vector
 * @brief Creates or fills a row vector with a specified value.
 *
 * - If `vector` is NULL → allocates new vector of size `size` and fills with `value`.
 * - If `vector` exists and matches `size` → fills with `value`.
 * - If size mismatches → sets error and returns NULL.
 *
 * @param size [in] Number of elements (must be > 0).
 * @param value [in] Value to assign to all elements.
 * @param vector [in,out] Optional existing vector to reuse, or NULL.
 * @return Pointer to valid vector on success, or NULL if an error occurred.
 *
 * @note Caller must free newly allocated vectors.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double *vec = dmlfw_row_vec_double_create_new_filled(10, 1.23, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating filled row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_row_vec_double *
dmlfw_row_vec_double_create_new_filled(dimension_t columns, double value, dmlfw_row_vec_double *vector);

/**
 * @ingroup vector
 * @brief Transposes a row vector into a column vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param transposed_vector [in,out] Optional existing column vector to reuse, or NULL.
 * @return Pointer to column vector, or NULL if an error occurred.
 *
 * @note Caller must free newly allocated column vectors.
 * @note Check errors using the centralized error API after calling this function.
 * @see dmlfw_column_vec_double_transpose
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *col_vec = dmlfw_row_vec_double_transpose(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing to column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_column_vec_double *
dmlfw_row_vec_double_transpose(dmlfw_row_vec_double *vector, dmlfw_column_vec_double *transposed_vector);

/**
 * @ingroup vector
 * @brief Returns the number of elements in a row vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @return Number of elements, or 0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 */
dimension_t
dmlfw_row_vec_double_get_size(dmlfw_row_vec_double *vector);

/**
 * @ingroup vector
 * @brief Computes the mean of all elements in a row vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @return Mean value, or 0.0 if an error occurred.
 *
 * @note Check errors using the centralized error API after calling this function.
 */
double
dmlfw_row_vec_double_get_mean(dmlfw_row_vec_double *vector);

/**
 * @ingroup vector
 * @brief Exports a row vector to a CSV file with a header.
 *
 * Writes the elements of the row vector to a CSV file specified by `csv_file_name`,
 * with a header row consisting of strings from the `header` row vector.
 *
 * Assumptions:
 * - Header size must exactly match the row vector size.
 * - File is overwritten if it exists.
 *
 * Error Cases:
 * - Invalid parameters (NULL vector, csv_file_name, or header).
 * - File creation fails.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param csv_file_name [in] Output CSV file path (must not be NULL).
 * @param header [in] Row vector of strings header (must not be NULL, size must match vector size).
 *
 * @note The file will be overwritten if it exists.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * @see dmlfw_row_vec_string
 * @see dmlfw_vec_string.h
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *header = dmlfw_row_vec_string_create_new(3);
 * dmlfw_row_vec_string_set(header, 0, "x");
 * dmlfw_row_vec_string_set(header, 1, "y");
 * dmlfw_row_vec_string_set(header, 2, "z");
 * dmlfw_row_vec_double_to_csv(vec, "row_data.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error exporting row vector to CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
void
dmlfw_row_vec_double_to_csv(dmlfw_row_vec_double *vector,const char *csv_file_name, dmlfw_row_vec_string *header);

/**
 * @ingroup vector
 * @brief Loads a row vector from a CSV file.
 *
 * Reads a CSV file specified by `csv_file_name` and populates a row vector.
 * The first line is parsed into a row vector of strings as header.
 *
 * Assumptions:
 * - Number of header strings must match the number of numeric data elements.
 * - If `vector` is NULL, a new row vector is allocated; otherwise, must match data size.
 *
 * Error Cases:
 * - Invalid parameters (NULL csv_file_name or header pointer).
 * - File cannot be opened.
 * - Header size mismatch.
 * - Provided vector size mismatch.
 * - Invalid numeric values in CSV.
 *
 * @param csv_file_name [in] Input CSV file path (must not be NULL).
 * @param vector [in,out] Optional existing row vector to reuse, or NULL.
 * @param header [out] Output pointer to a newly allocated row vector containing header strings.
 * @return Pointer to the populated row vector, or NULL if an error occurred.
 *
 * @note Caller must destroy both the returned vector and header.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * @see dmlfw_row_vec_string
 * @see dmlfw_vec_string.h
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double *vec = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * vec = dmlfw_row_vec_double_from_csv("row_data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error loading row vector from CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * // use vec and header...
 * dmlfw_row_vec_double_destroy(vec);
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
dmlfw_row_vec_double *
dmlfw_row_vec_double_from_csv(const char *csv_file_name,
                              dmlfw_row_vec_double *vector,
                              dmlfw_row_vec_string **header);

/**
 * @ingroup vector
 * @brief Copies the contents of one row vector to another.
 *
 * Assumptions:
 * - Both vectors must be non-NULL and have the same size.
 *
 * Error Cases:
 * - NULL target or source vectors.
 * - Size mismatch between vectors.
 *
 * @param target_vector [in,out] Destination row vector (must not be NULL).
 * @param source_vector [in] Source row vector (must not be NULL).
 *
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double_copy(target, source);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error copying row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void
dmlfw_row_vec_double_copy(dmlfw_row_vec_double *target_vector,
                          dmlfw_row_vec_double *source_vector);


#endif // __DMLFW__VEC__DOUBLE__

/** @} */ // end of vector group
