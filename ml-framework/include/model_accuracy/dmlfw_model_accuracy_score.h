#ifndef __DMLFW__MODEL__ACCURACY__SCORE__
#define __DMLFW__MODEL__ACCURACY__SCORE__
/**
 * @file dmlfw_model_accuracy_score.h
 * @brief Regression model evaluation metrics.
 * @ingroup model_accuracy_score
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This header provides functions to evaluate the accuracy of regression models
 * using numerical metrics such as the R2 score (coefficient of determination).
 *
 * Error Handling:
 * ---------------
 * Functions validate inputs, check vector sizes, allocate necessary temporaries,
 * and set errors on failure. After calling, check `dmlfw_error()` and
 * use `dmlfw_get_error_string()` and `dmlfw_get_debug_string()` for diagnostics.
 *
 * Ownership:
 * ----------
 * Input vectors are owned by the caller; temporary allocations are freed internally.
 * Returned scores are scalars.
 * @example example_dmlfw_model_accuracy_score.c
 * Usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_model_accuracy.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_column_vec_double *target = dmlfw_column_vec_double_create_new(3);
 *     dmlfw_column_vec_double *predicted = dmlfw_column_vec_double_create_new(3);
 *
 *     dmlfw_column_vec_double_set(target, 0, 3.0);
 *     dmlfw_column_vec_double_set(target, 1, -0.5);
 *     dmlfw_column_vec_double_set(target, 2, 2.0);
 *
 *     dmlfw_column_vec_double_set(predicted, 0, 2.5);
 *     dmlfw_column_vec_double_set(predicted, 1, 0.0);
 *     dmlfw_column_vec_double_set(predicted, 2, 2.0);
 *
 *     double r2 = dmlfw_get_r2_score(target, predicted);
 *
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Error computing R2 score: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_column_vec_double_destroy(target);
 *         dmlfw_column_vec_double_destroy(predicted);
 *         return EXIT_FAILURE;
 *     }
 *
 *     printf("R2 score: %lf\n", r2);
 *
 *     dmlfw_column_vec_double_destroy(target);
 *     dmlfw_column_vec_double_destroy(predicted);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */
#include <dmlfw_vector.h>
#include <dmlfw_error.h>


/**
 * @brief Computes the R2 (coefficient of determination) score for regression.
 *
 * The R2 score is calculated as:
 * \f[
 * R^2 = 1 - \frac{\sum_i (y_i - \hat{y}_i)^2}{\sum_i (y_i - \bar{y})^2}
 * \f]
 *
 * @param target_values_vector [in] Pointer to true target value vector (non-NULL).
 * @param predicted_values_vector [in] Pointer to predicted value vector (non-NULL).
 *
 * @return R2 score as double.
 *         Returns 0.0 and sets an appropriate error if inputs are invalid
 *         or computation fails.
 *
 * @note Input vectors must be the same size.
 * @note Caller retains ownership of input vectors.
 *
 * Usage:
 * @code
 * dmlfw_column_vec_double *target = ...;   // Initialized true values
 * dmlfw_column_vec_double *predicted = ...;// Initialized predictions
 * double r2 = dmlfw_get_r2_score(target, predicted);
 * if (dmlfw_error()) {
 *     char err[512], dbg[512];
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error computing R2 score: %s\nDebug info: %s\n", err, dbg);
 * }
 * printf("R2 score: %lf\n", r2);
 * @endcode
 */
double dmlfw_get_r2_score(dmlfw_column_vec_double *target_values_vector, 
                          dmlfw_column_vec_double *predicted_values_vector);

/** @} */ // end of group
#endif // __DMLFW__MODEL__ACCURACY__SCORE__
