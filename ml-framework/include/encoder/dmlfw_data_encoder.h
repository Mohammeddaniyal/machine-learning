#ifndef __DMLFW__DATA__ENCODER__
#define __DMLFW__DATA__ENCODER__
/**
 * @file dmlfw_data_encoder.h
 * @defgroup data_encoder Feature Encoding
 * @ingroup encoder
 * @brief Categorical and binary feature encoding utilities.
 *
 * Version: 1.0
 * Date: 2025-09-25
 *
 * @{
 *
 * This module provides functions for encoding categorical/textual data
 * columns from CSV files into numerical formats suitable for ML models.
 * Supported encoding schemes include one-hot and binary encoding, 
 * with a generic dispatcher for algorithm selection.
 *
 * Error Handling:
 * ----------------
 * All functions report errors via the centralized error API.
 * Use `dmlfw_error()` after function calls to check for errors.
 * Retrieve error and diagnostic details with `dmlfw_get_error_string()` and `dmlfw_get_debug_string()`.
 *
 * Ownership:
 * ----------------
 * All allocated resources are cleaned up internally on error and
 * after completion. No ownership is transferred to caller.
 *
 * @example example_dmlfw_data_encoder.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_encoder.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_row_vec_string *columns = dmlfw_row_vec_string_create_new(2);
 *     dmlfw_row_vec_string_set(columns, 0, "town");
 *     dmlfw_row_vec_string_set(columns, 1, "color");
 *     dmlfw_encoder_encode_one_hot("input.csv", "onehot.csv", columns);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error in one-hot encoding: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_row_vec_string_destroy(columns);
 *         return EXIT_FAILURE;
 *     }
 *     dmlfw_row_vec_string_destroy(columns);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */

#include <dmlfw_vector.h>

/**
 * @brief Encodes specified columns in a CSV file using one-hot encoding.
 *
 * @param source [in] Input CSV file path (must not be NULL).
 * @param target [in] Output CSV file path (must not be NULL).
 * @param columns_to_encode [in] Vector of column names to encode (must not be NULL).
 * 
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *columns = dmlfw_row_vec_string_create_new(2);
 * dmlfw_row_vec_string_set(columns, 0, "size");
 * dmlfw_row_vec_string_set(columns, 1, "shape");
 * dmlfw_encoder_encode_one_hot("data.csv", "ohe.csv", columns);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in one-hot encoding: %s\nDebug info: %s\n", err, dbg);
 *     dmlfw_row_vec_string_destroy(columns);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_row_vec_string_destroy(columns);
 * @endcode
 */
void dmlfw_encoder_encode_one_hot(char *source, char *target, dmlfw_row_vec_string *columns_to_encode);

/**
 * @brief Encodes specified columns in a CSV file using binary encoding.
 *
 * @param source [in] Input CSV file path (must not be NULL).
 * @param target [in] Output CSV file path (must not be NULL).
 * @param columns_to_encode [in] Vector of column names to encode (must not be NULL).
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *columns = dmlfw_row_vec_string_create_new(1);
 * dmlfw_row_vec_string_set(columns, 0, "product");
 * dmlfw_encoder_encode_binary("data.csv", "binary.csv", columns);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in binary encoding: %s\nDebug info: %s\n", err, dbg);
 *     dmlfw_row_vec_string_destroy(columns);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_row_vec_string_destroy(columns);
 * @endcode
 */
void dmlfw_encoder_encode_binary(char *source, char *target, dmlfw_row_vec_string *columns_to_encode);

/**
 * @brief Generic encoder API. Dispatches by algorithm string.
 *
 * @param source [in] Input CSV file path (must not be NULL).
 * @param target [in] Output CSV file path (must not be NULL).
 * @param columns_to_encode [in] Vector of column names to encode (must not be NULL).
 * @param algorithm [in] Algorithm name ("one-hot" or "binary", case-insensitive).
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_string *cols = dmlfw_row_vec_string_create_new(1);
 * dmlfw_row_vec_string_set(cols, 0, "feature");
 * dmlfw_encoder_encode("data.csv", "encoded.csv", cols, "binary");
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error in encoding: %s\nDebug info: %s\n", err, dbg);
 *     dmlfw_row_vec_string_destroy(cols);
 *     return EXIT_FAILURE;
 * }
 * dmlfw_row_vec_string_destroy(cols);
 * @endcode
 */
void dmlfw_encoder_encode(char *source, char *target, dmlfw_row_vec_string *columns_to_encode, char *algorithm);
/** @} */ ///< End of data_encoder group
#endif // __DMLFW__DATA__ENCODER__
