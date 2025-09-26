/**
 * @file dmlfw_vec_string.h
 * @brief String row and column vector types and operations.
 *
 * This header defines opaque vector data structures for string values.
 * It provides APIs for creating, destroying, accessing, modifying,
 * and manipulating both column and row string vectors.
 *
 * Common operations include:
 * - Creation and destruction
 * - Element access and mutation
 * - Transposing between row and column vectors
 * - CSV import/export
 * - Size retrieval
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
 * - On error, functions return NULL or 0 depending on return type.
 *
 * Version: 1.0
 * Date: 2025-09-25
 *
 * @defgroup vector String Vectors
 * @ingroup vector
 * @brief String column and row vector types and functions.
 *
 * @example example_usage_string.c
 * Typical usage example:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_vec_string.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_row_vec_string *row_vec = dmlfw_row_vec_string_create_new(3);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error creating row vector: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_row_vec_string_set(row_vec, 0, "Hello");
 *     dmlfw_row_vec_string_set(row_vec, 1, "World");
 *     dmlfw_row_vec_string_set(row_vec, 2, "!");
 *
 *     char *val = NULL;
 *     dmlfw_row_vec_string_get(row_vec, 1, &val);
 *     if (!dmlfw_error() && val) {
 *         printf("Element at index 1: %s\n", val);
 *     }
 *
 *     dmlfw_row_vec_string_destroy(row_vec);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * @{
 */

#ifndef __DMLFW__VEC__STRING__
#define __DMLFW__VEC__STRING__

#include <dmlfw_types.h>

struct __dmlfw_row_vec_string;
struct __dmlfw_column_vec_string;

/**
 * @ingroup vector
 * @brief Opaque structure representing a row vector of strings.
 *
 * The internal details are hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_row_vec_string dmlfw_row_vec_string;

/**
 * @ingroup vector
 * @brief Opaque structure representing a column vector of strings.
 *
 * The internal details are hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_column_vec_string dmlfw_column_vec_string;


//
// Row vector API
//

/**
 * @brief Creates a new row vector of strings of specified length.
 *
 * @param columns [in] Number of string elements (must be > 0).
 * @return Pointer to a new row vector, or NULL on error.
 *
 * @note Caller must free with dmlfw_row_vec_string_destroy().
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *vec = dmlfw_row_vec_string_create_new(5);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_row_vec_string * dmlfw_row_vec_string_create_new(dimension_t columns);

/**
 * @brief Destroys a row vector of strings and frees its memory.
 *
 * @param vector [in] Pointer to the row vector, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_row_vec_string_destroy(vec);
 * @endcode
 */
void dmlfw_row_vec_string_destroy(dmlfw_row_vec_string *vector);

/**
 * @brief Loads a row vector of strings from a CSV file.
 *
 * @param csv_file_name [in] Path to CSV file (must not be NULL).
 * @param vector [in,out] Optional existing vector to reuse, or NULL.
 * @param header [out] Output pointer to a newly allocated header vector.
 * @return Pointer to the loaded row vector, or NULL on error.
 *
 * @note Caller must free both the returned vector and the header.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *vec = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * vec = dmlfw_row_vec_string_from_csv("data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error loading from CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * // Use vec and header...
 * dmlfw_row_vec_string_destroy(vec);
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
dmlfw_row_vec_string * dmlfw_row_vec_string_from_csv(const char *csv_file_name,
                                                    dmlfw_row_vec_string *vector,
                                                    dmlfw_row_vec_string **header);

/**
 * @brief Exports a row vector of strings to a CSV file with a header.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param csv_file_name [in] Output CSV file path (must not be NULL).
 * @param header [in] Row vector of strings header (must not be NULL).
 *
 * @note The file will be overwritten if it exists.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string_to_csv(vec, "out.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error writing to CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_row_vec_string_to_csv(dmlfw_row_vec_string *vector,
                                 const char *csv_file_name,
                                 dmlfw_row_vec_string *header);

/**
 * @brief Retrieves a string element from a row vector by index.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param index [in] Zero-based index.
 * @param string [out] Pointer to receive the string element.
 *
 * @note The returned string pointer is managed by the vector and must not be freed by the caller.
 *
 * Usage example:
 * @code
 * char *val = NULL;
 * dmlfw_row_vec_string_get(vec, 2, &val);
 * if (dmlfw_error()) {
 *     char err[512], dbg[512];
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to retrieve value at index 2: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * printf("Element at index 2: %s\n", val);
 * @endcode
 */
void dmlfw_row_vec_string_get(dmlfw_row_vec_string *vector,
                              index_t index,
                              char **string);

/**
 * @brief Sets a string element in the row vector.
 *
 * @param vector [in,out] Row vector (must not be NULL).
 * @param index [in] Zero-based index.
 * @param string [in] Input string to copy into the vector (caller retains ownership).
 *
 * Usage example:
 * @code
 * dmlfw_row_vec_string_set(vec, 0, "example");
 * @endcode
 */
void dmlfw_row_vec_string_set(dmlfw_row_vec_string *vector,
                              index_t index,
                              char *string);

/**
 * @brief Returns the size (number of elements) in a row vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @return Number of elements, or 0 on error.
 */
dimension_t dmlfw_row_vec_string_get_size(dmlfw_row_vec_string *vector);

/**
 * @brief Transposes a row vector of strings into a column vector.
 *
 * @param vector [in] Row vector (must not be NULL).
 * @param transposed_vector [in,out] Optional existing column vector to reuse, or NULL.
 * @return Pointer to new or reused column vector, or NULL on error.
 *
 * @note Caller must free newly allocated column vectors.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_string *col_vec = dmlfw_row_vec_string_transpose(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing to column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_column_vec_string * dmlfw_row_vec_string_transpose(dmlfw_row_vec_string *vector,
                                                        dmlfw_column_vec_string *transposed_vector);

//
// Column vector API
//

/**
 * @brief Creates a new column vector of strings of specified length.
 *
 * @param rows [in] Number of string elements (must be > 0).
 * @return Pointer to a new column vector, or NULL on error.
 *
 * @note Caller must free with dmlfw_column_vec_string_destroy().
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_string *vec = dmlfw_column_vec_string_create_new(5);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error creating column vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_column_vec_string * dmlfw_column_vec_string_create_new(dimension_t rows);

/**
 * @brief Destroys a column vector of strings and frees its memory.
 *
 * @param vector [in] Pointer to the column vector, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_column_vec_string_destroy(vec);
 * @endcode
 */
void dmlfw_column_vec_string_destroy(dmlfw_column_vec_string *vector);

/**
 * @brief Loads a column vector of strings from a CSV file.
 *
 * @param csv_file_name [in] Path to CSV file (must not be NULL).
 * @param vector [in,out] Optional existing vector to reuse, or NULL.
 * @param header [out] Output pointer to a newly allocated row vector header.
 * @return Pointer to the loaded column vector, or NULL on error.
 *
 * @note Caller must free both the returned vector and the header.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_string *vec = NULL;
 * dmlfw_row_vec_string *header = NULL;
 * vec = dmlfw_column_vec_string_from_csv("data.csv", NULL, &header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error loading from CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_column_vec_string_destroy(vec);
 * dmlfw_row_vec_string_destroy(header);
 * @endcode
 */
dmlfw_column_vec_string * dmlfw_column_vec_string_from_csv(const char *csv_file_name,
                                                          dmlfw_column_vec_string *vector,
                                                          dmlfw_row_vec_string **header);

/**
 * @brief Exports a column vector of strings to a CSV file with a header.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param csv_file_name [in] Output CSV file path (must not be NULL).
 * @param header [in] Row vector of string header (must not be NULL).
 *
 * @note The file will be overwritten if it exists.
 * @note The `header` is always a row vector, even when representing a column vector's header.
 * @note Check errors using the centralized error API after calling this function.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_string_to_csv(vec, "out.csv", header);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error writing to CSV: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_column_vec_string_to_csv(dmlfw_column_vec_string *vector,
                                    const char *csv_file_name,
                                    dmlfw_row_vec_string *header);

/**
 * @brief Retrieves a string element from a column vector by index.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param index [in] Zero-based index.
 * @param string [out] Pointer to receive the string element.
 *
 * @note The returned string pointer is managed by the vector and must not be freed by the caller.
 *
 * Usage example:
 * @code
 * char *val = NULL;
 * dmlfw_column_vec_string_get(vec, 0, &val);
 * if (dmlfw_error()) {
 *     char err[512], dbg[512];
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error retrieving value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * printf("Element at index 0: %s\n", val);
 * @endcode
 */
void dmlfw_column_vec_string_get(dmlfw_column_vec_string *vector,
                                index_t index,
                                char **string);

/**
 * @brief Sets a string element in the column vector.
 *
 * @param vector [in,out] Column vector (must not be NULL).
 * @param index [in] Zero-based index.
 * @param string [in] Input string to copy into the vector (caller retains ownership).
 *
 * Usage example:
 * @code
 * dmlfw_column_vec_string_set(vec, 1, "example");
 * @endcode
 */
void dmlfw_column_vec_string_set(dmlfw_column_vec_string *vector,
                                index_t index,
                                char *string);

/**
 * @brief Returns the size (number of elements) in a column vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @return Number of elements, or 0 on error.
 */
dimension_t dmlfw_column_vec_string_get_size(dmlfw_column_vec_string *vector);

/**
 * @brief Transposes a column vector of strings into a row vector.
 *
 * @param vector [in] Column vector (must not be NULL).
 * @param transposed_vector [in,out] Optional existing row vector to reuse, or NULL.
 * @return Pointer to new or reused row vector, or NULL on error.
 *
 * @note Caller must free newly allocated row vectors.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *row_vec = dmlfw_column_vec_string_transpose(vec, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error transposing to row vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_row_vec_string * dmlfw_column_vec_string_transpose(dmlfw_column_vec_string *vector,
                                                        dmlfw_row_vec_string *transposed_vector);

#endif // __DMLFW__VEC__STRING__

/** @} */ // end of vector group
