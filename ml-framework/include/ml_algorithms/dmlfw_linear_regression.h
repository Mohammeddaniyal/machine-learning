#ifndef __DMLFW__LINEAR_REGRESSION__
#define __DMLFW__LINEAR_REGRESSION__
/**
 * @file dmlfw_linear_regression.h
 * @defgroup linear_regression Linear Regression
 * @ingroup algorithms
 * @brief Gradient descent-based linear regression training and prediction.
 *
 * Version: 1.0
 * Date: 2025-09-25
 *
 * @{
 *
 * This module provides APIs to fit linear regression models using
 * batch, stochastic, and mini-batch gradient descent methods, including support
 * for regularization and user-defined progress callbacks.
 * It also offers prediction functionality from trained models.
 *
 * Error Handling:
 * ---------------
 * Functions use centralized error reporting. Call `dmlfw_error()` after function execution
 * to check for errors. For error details, use `dmlfw_get_error_string()` and
 * `dmlfw_get_debug_string()`.
 *
 * Ownership:
 * ----------
 * Returned vectors representing trained models or predictions must be freed
 * using the vector destroy API to avoid memory leaks.
 * 
 *
 * @example example_dmlfw_linear_regression.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_algorithms.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_gradient_descent_options *opts = dmlfw_gradient_descent_options_create_new();
 *     dmlfw_mat_double *x; // Assume initialized input feature matrix
 *     dmlfw_column_vec_double *y; // Assume initialized target vector
 *
 *     dmlfw_gradient_descent_options_set_learning_rate(opts, 0.01);
 *     dmlfw_gradient_descent_options_set_number_of_iterations(opts, 1000);
 *
 *     dmlfw_column_vec_double *model = dmlfw_linear_regression_fit_using_batch_gradient_descent(
 *         opts, x, y, 0.1, NULL);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Training error: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_gradient_descent_options_destroy(opts);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_column_vec_double *predictions = dmlfw_linear_regression_predict(x, model);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Prediction error: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_column_vec_double_destroy(model);
 *         dmlfw_gradient_descent_options_destroy(opts);
 *         return EXIT_FAILURE;
 *     }
 *
 *     // Use predictions...
 *
 *     dmlfw_column_vec_double_destroy(predictions);
 *     dmlfw_column_vec_double_destroy(model);
 *     dmlfw_gradient_descent_options_destroy(opts);
 *
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */


#include <dmlfw_vector.h>
#include <dmlfw_matrix.h>
#include <dmlfw_algorithms.h>

/**
 * @brief Fits linear regression model using batch gradient descent.
 *
 * @param gd_options [in] Gradient descent options (non-NULL).
 * @param x [in] Feature matrix (non-NULL).
 * @param y [in] Target vector (non-NULL).
 * @param regularization_parameter [in] L2 regularization coefficient.
 * @param model [in,out] Optional initial model vector or NULL.
 * @return Pointer to trained model vector or NULL on error.
 *
 * @see dmlfw_linear_regression_predict
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *model = dmlfw_linear_regression_fit_using_batch_gradient_descent(
 *     opts, x, y, 0.1, NULL);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error training model: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use model...
 * dmlfw_column_vec_double_destroy(model);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_linear_regression_fit_using_batch_gradient_descent(
    dmlfw_gradient_descent_options *gd_options,
    dmlfw_mat_double *x,
    dmlfw_column_vec_double *y,
    double regularization_parameter,
    dmlfw_column_vec_double *model);

/**
 * @brief Predicts output using trained linear regression model.
 *
 * @param x [in] Feature matrix (non-NULL).
 * @param model [in] Trained model vector (non-NULL).
 * @return Pointer to prediction vector or NULL on error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_column_vec_double *predictions = dmlfw_linear_regression_predict(x, model);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error predicting output: %s\nDebug info: %s\n", err, dbg);
 *     return NULL;
 * }
 * // Use predictions...
 * dmlfw_column_vec_double_destroy(predictions);
 * @endcode
 */
dmlfw_column_vec_double * dmlfw_linear_regression_predict(
    dmlfw_mat_double *x,
    dmlfw_column_vec_double *model);

/**
 * @brief Fits linear regression model using stochastic gradient descent.
 *
 * @param gd_options [in] Gradient descent options (non-NULL).
 * @param regularization_parameter [in] L2 regularization coefficient.
 * @param model [in,out] Optional initial model vector or NULL.
 * @return Pointer to trained model vector or NULL on error.
 */
dmlfw_column_vec_double * dmlfw_linear_regression_fit_using_stochastic_gradient_descent(
    dmlfw_gradient_descent_options *gd_options,
    double regularization_parameter,
    dmlfw_column_vec_double *model);

/**
 * @brief Fits linear regression model using mini-batch gradient descent.
 *
 * @param gd_options [in] Gradient descent options (non-NULL).
 * @param regularization_parameter [in] L2 regularization coefficient.
 * @param model [in,out] Optional initial model vector or NULL.
 * @return Pointer to trained model vector or NULL on error.
 */
dmlfw_column_vec_double * dmlfw_linear_regression_fit_using_mini_batch_gradient_descent(
    dmlfw_gradient_descent_options *gd_options,
    double regularization_parameter,
    dmlfw_column_vec_double *model);

/** @} */ ///< End of linear_regression group

#endif // __DMLFW__LINEAR_REGRESSION__
