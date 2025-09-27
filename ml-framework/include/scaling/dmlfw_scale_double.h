#ifndef __DMLFW__SCALE__DOUBLE__
#define __DMLFW__SCALE__DOUBLE__
/**
 * @file dmlfw_scale_double.h
 * @brief Matrix data scaling and normalization using double precision.
 * @ingroup scale_double
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This module provides scaling algorithms applied to double precision matrices,
 * such as min-max normalization and z-score standardization.
 *
 * Algorithms include:
 * - Min-Max scaling (rescale to [0,1])
 * - Z-Score scaling (mean 0, standard deviation 1)
 *
 * Functions support loading scaling parameters from files and applying previously
 * computed parameters to new data.
 *
 * Error Handling:
 * ---------------
 * All functions validate input parameters and interoperability.
 * Errors are reported via the centralized error API,
 * using `dmlfw_error()`, `dmlfw_get_error_string()`, and `dmlfw_get_debug_string()`.
 *
 * Ownership:
 * ----------
 * Caller owns all input matrices. Output matrices are allocated if `NULL` is passed,
 * and ownership passes to the caller, who must free them.
 *
 * @example example_dmlfw_scale_double.c
 * Usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_scale.h>
 * #include <dmlfw_vector.h>
 * #include <dmlfw_error.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *
 *     // Create a vector of column names to scale
 *     dmlfw_row_vec_string *columns_to_scale = dmlfw_row_vec_string_create_new(2);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error creating columns vector: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *     dmlfw_row_vec_string_set(columns_to_scale, 0, "feature1");
 *     dmlfw_row_vec_string_set(columns_to_scale, 1, "feature2");
 *
 *     // Perform min-max scaling on dataset columns, save parameters to "params.json"
 *     dmlfw_mat_double *scaled_matrix = dmlfw_scale_double("input.csv", columns_to_scale, "params.json",
 *                                                         MLFW_MIN_MAX_SCALING_ALGORITHM, NULL);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Scaling error: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_row_vec_string_destroy(columns_to_scale);
 *         return EXIT_FAILURE;
 *     }
 *
 *     // Use scaled_matrix for further processing...
 *     printf("Scaling completed successfully.\n");
 *
 *     // Cleanup
 *     dmlfw_mat_double_destroy(scaled_matrix);
 *     dmlfw_row_vec_string_destroy(columns_to_scale);
 *
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */
#include <dmlfw_matrix.h>

/**
 * Min-Max scaling algorithm identifier string.
 */
#define MLFW_MIN_MAX_SCALING_ALGORITHM "min-max"

/**
 * Z-Score scaling algorithm identifier string.
 */
#define MLFW_Z_SCORE_SCALING_ALGORITHM "z-score"


/**
 * @brief Scales data from a dataset according to specified columns and algorithm.
 *
 * @param dataset_file_name [in] File path of the dataset (CSV or supported format).
 * @param columns_to_scale [in] Row vector of column names/labels to scale.
 * @param parameters_file_name [in] Path to save or read scaling parameters.
 * @param algorithm [in] Scaling algorithm identifier: "min-max" or "z-score".
 * @param matrix [in,out] Optional pre-allocated matrix to store scaled data,
 *                       or NULL to allocate a new one.
 * @return Pointer to the scaled matrix or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_mat_double *scaled = dmlfw_scale_double("input.csv", cols_to_scale, "params.json", MLFW_MIN_MAX_SCALING_ALGORITHM, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Scaling error: %s\nDebug: %s\n", err, dbg);
 *     return NULL;
 * }
 * // use scaled...
 * dmlfw_mat_double_destroy(scaled);
 * @endcode
 */
dmlfw_mat_double *
dmlfw_scale_double(char *dataset_file_name, 
                  dmlfw_row_vec_string *columns_to_scale,
                  char *parameters_file_name,
                  char *algorithm,
                  dmlfw_mat_double *matrix);


/**
 * @brief Scales data using pre-computed parameters according to algorithm.
 *
 * @param dataset_file_name [in] Dataset source file path.
 * @param parameters_file_name [in] File for reading pre-computed scaling parameters.
 * @param algorithm [in] Scaling algorithm identifier.
 * @param matrix [in,out] Optional matrix to output scaled data, or NULL to allocate.
 * @return Pointer to scaled matrix or NULL on error.
 */
dmlfw_mat_double *
dmlfw_scale_double_with_given_parameters(char *dataset_file_name,
                                        char *parameters_file_name,
                                        char *algorithm,
                                        dmlfw_mat_double *matrix);


/**
 * @brief Performs min-max scaling on a submatrix slice and optionally returns min-max matrix.
 *
 * @param matrix [in] Input matrix.
 * @param start_row_index [in] Start index for rows (inclusive).
 * @param start_column_index [in] Start index for columns (inclusive).
 * @param end_row_index [in] End index for rows (inclusive).
 * @param end_column_index [in] End index for columns (inclusive).
 * @param min_max_matrix [out] Optional pointer to store min and max values matrix,
 *                           allocated if NULL.
 * @param scaled_matrix [in,out] Optional matrix to store the scaled data,
 *                              or NULL to allocate a new matrix.
 * @return Pointer to scaled matrix or NULL on error.
 */
dmlfw_mat_double *
dmlfw_scale_double_min_max(dmlfw_mat_double *matrix,
                          index_t start_row_index,
                          index_t start_column_index,
                          index_t end_row_index,
                          index_t end_column_index,
                          dmlfw_mat_double **min_max_matrix,
                          dmlfw_mat_double *scaled_matrix);


/**
 * @brief Scales matrix using given min-max matrix for normalization.
 *
 * @param matrix [in] Input matrix to scale.
 * @param start_row_index [in] Start row index for submatrix.
 * @param start_column_index [in] Start column index for submatrix.
 * @param end_row_index [in] End row index for submatrix.
 * @param end_column_index [in] End column index for submatrix.
 * @param min_max_matrix [in] Matrix containing min and max values per column.
 * @param scaled_matrix [in,out] Optional matrix to hold scaled result, or NULL for allocation.
 * @return Pointer to scaled matrix or NULL on error.
 */
dmlfw_mat_double *
dmlfw_scale_double_with_given_min_max(dmlfw_mat_double *matrix,
                                     index_t start_row_index,
                                     index_t start_column_index,
                                     index_t end_row_index,
                                     index_t end_column_index,
                                     dmlfw_mat_double *min_max_matrix,
                                     dmlfw_mat_double *scaled_matrix);


/**
 * @brief Performs z-score scaling on a submatrix slice, optionally returning mean/std matrix.
 *
 * @param matrix [in] Input matrix.
 * @param start_row_index [in] Start row index (inclusive).
 * @param start_column_index [in] Start column index (inclusive).
 * @param end_row_index [in] End row index (inclusive).
 * @param end_column_index [in] End column index (inclusive).
 * @param mean_standard_deviation_matrix [out] Optional pointer for mean and std dev,
 *                                           allocated if NULL.
 * @param scaled_matrix [in,out] Matrix to store scaled data or NULL to allocate.
 * @return Pointer to scaled matrix or NULL on error.
 */
dmlfw_mat_double *
dmlfw_scale_double_z_score(dmlfw_mat_double *matrix,
                          index_t start_row_index,
                          index_t start_column_index,
                          index_t end_row_index,
                          index_t end_column_index,
                          dmlfw_mat_double **mean_standard_deviation_matrix,
                          dmlfw_mat_double *scaled_matrix);


/**
 * @brief Scales matrix using given mean and standard deviation matrix.
 *
 * @param matrix [in] Input matrix.
 * @param start_row_index [in] Start row index for scaling.
 * @param start_column_index [in] Start column index for scaling.
 * @param end_row_index [in] End row index for scaling.
 * @param end_column_index [in] End column index for scaling.
 * @param mean_standard_deviation_matrix [in] Matrix with mean and std deviation.
 * @param scaled_matrix [in,out] Matrix for scaled output or NULL to allocate.
 * @return Pointer to scaled matrix or NULL on error.
 */
dmlfw_mat_double *
dmlfw_scale_double_z_score_with_given_mean_standard_deviation(dmlfw_mat_double *matrix,
                                                             index_t start_row_index,
                                                             index_t start_column_index,
                                                             index_t end_row_index,
                                                             index_t end_column_index,
                                                             dmlfw_mat_double *mean_standard_deviation_matrix,
                                                             dmlfw_mat_double *scaled_matrix);


/** @} */ // end of group
#endif // __DMLFW__SCALE__DOUBLE__
