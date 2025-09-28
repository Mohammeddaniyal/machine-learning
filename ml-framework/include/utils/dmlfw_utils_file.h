#ifndef __DMLFW__UTILS__FILE__
#define __DMLFW__UTILS__FILE__
/**
 * @file dmlfw_utils_file.h
 * @brief Functions for CSV file dimensions and inspection.
 * @ingroup utils_file
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This header provides functions to retrieve dimension information from CSV files,
 * such as the number of rows, columns, and both dimensions together.
 *
 * Error Handling:
 * ---------------
 * Functions set error status on failure using the centralized error API.
 * Use `dmlfw_error()` to check error status after calls.
 * Use `dmlfw_get_error_string()` and `dmlfw_get_debug_string()` for details.
 *
 * @example example_dmlfw_utils_file.c
 * Typical usage:
 * @code
 * #include <stdio.h>
 * #include <dmlfw_utils.h>
 * #include <dmlfw_error.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     const char *filename = "data.csv";
 *     
 *     uint64_t rows = dmlfw_get_csv_rows_count(filename);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error counting rows: %s\nDebug info: %s\n", err, dbg);
 *         return 1;
 *     }
 *     
 *     uint64_t cols = dmlfw_get_csv_columns_count(filename);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error counting columns: %s\nDebug info: %s\n", err, dbg);
 *         return 1;
 *     }
 *     
 *     printf("CSV '%s' has %llu rows and %llu columns\n", filename, rows, cols);
 *     
 *     uint64_t rows2, cols2;
 *     dmlfw_get_csv_dimensions(filename, &rows2, &cols2);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error getting dimensions: %s\nDebug info: %s\n", err, dbg);
 *         return 1;
 *     }
 *     
 *     printf("Dimensions verified: %llu rows, %llu columns\n", rows2, cols2);
 *     return 0;
 * }
 * @endcode
 */

/**
 * @brief Counts the number of rows in a CSV file.
 *
 * Counts all lines except possibly header depending on implementation.
 *
 * @param csv_file_name [in] Path to the CSV file to be analyzed.
 * @return Number of rows in the CSV file, or 0 on failure (check error).
 */
uint64_t dmlfw_get_csv_rows_count(const char *csv_file_name);


/**
 * @brief Counts the number of columns in a CSV file.
 *
 * Determines the number of columns in the first data row (or header).
 *
 * @param csv_file_name [in] Path to the CSV file to be analyzed.
 * @return Number of columns in the CSV file, or 0 on failure (check error).
 */
uint64_t dmlfw_get_csv_columns_count(const char *csv_file_name);


/**
 * @brief Retrieves both the row and column counts of a CSV file.
 *
 * Reads the CSV file and fills in the number of rows and columns.
 *
 * @param csv_file_name [in] Path to the CSV file.
 * @param rows [out] Pointer to store the number of rows.
 * @param columns [out] Pointer to store the number of columns.
 */
void dmlfw_get_csv_dimensions(const char *csv_file_name, uint64_t *rows, uint64_t *columns);

#endif // __DMLFW__UTILS__FILE__
/** @} */ // end of group
