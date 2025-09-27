#ifndef __DMLFW__ML_CONFIGURATIONS__
#define __DMLFW__ML_CONFIGURATIONS__
/**
 * @file dmlfw_ml_configurations.h
 * @brief Gradient descent configuration data structures and APIs.
 * @ingroup configurations
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This module provides definitions and management functions for gradient descent
 * configuration objects used with ML algorithms such as linear regression.
 * It enables detailed customization of learning rate, iterations, gradient descent type,
 * progress callbacks, data provisioning, and mini-batch processing.
 *
 * Error Handling:
 * ---------------
 * All functions validate input parameters and report errors via the centralized error API.
 * After function calls, check `dmlfw_error()` to detect if an error occurred.
 * Retrieve descriptive error messages with `dmlfw_get_error_string()` and diagnostic info
 * with `dmlfw_get_debug_string()`.
 *
 *
 * @example example_dmlfw_ml_configurations.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_algorithms.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_gradient_descent_options *opts = dmlfw_gradient_descent_options_create_new();
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Creation failed: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_gradient_descent_options_set_learning_rate(opts, 0.01);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Setting learning rate failed: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_gradient_descent_options_destroy(opts);
 *         return EXIT_FAILURE;
 *     }
 *
 *     // Additional configuration calls with error checks...
 *
 *     dmlfw_gradient_descent_options_destroy(opts);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * Ownership:
 * ----------
 * Creation functions allocate new configuration objects; callers must free
 * them using the provided destroy function.
 *
 */


/**
 * @brief Gradient descent algorithm types.
 */
enum MLFW_GRADIENT_DESCENT_TYPE {
    MLFW_BATCH_GRADIENT_DESCENT,       ///< Full batch gradient descent.
    MLFW_STOCHASTIC_GRADIENT_DESCENT,  ///< Stochastic gradient descent.
    MLFW_MINI_BATCH_GRADIENT_DESCENT   ///< Mini-batch gradient descent.
};


/**
 * @brief Progress callback for gradient descent linear regression.
 *
 * Called during each iteration to report training status.
 *
 * @param iteration_number Current iteration count.
 * @param y Actual target values.
 * @param predicted_y Predicted target values.
 * @param model Current model parameters.
 * @param regularization_parameter Regularization term used.
 *
 * @return Zero to continue training, negative to terminate early.
 */
typedef int (*dmlfw_gradient_descent_lin_reg_progress_callback_t)(
    uint64_t iteration_number,
    void *y,
    void *predicted_y,
    void *model,
    double regularization_parameter);


/**
 * @brief Data provider callback for gradient descent linear regression.
 *
 * Supplies batches of data for training.
 *
 * @param x Feature matrix subset.
 * @param y Target vector subset.
 * @param from_row_index Index of first row in batch.
 * @param how_many_rows Number of rows in batch.
 */
typedef void (*dmlfw_gradient_descent_lin_reg_data_provider_t)(
    void *x,
    void *y,
    uint64_t from_row_index,
    uint32_t how_many_rows);


/**
 * @brief Opaque structure representing gradient descent configuration options.
 *
 * The internal details are hidden. Use the provided API functions
 * to create, manipulate, and destroy instances.
 */
typedef struct _dmlfw_gradient_descent_options dmlfw_gradient_descent_options;


/**
 * @brief Creates a new gradient descent options object with default values.
 *
 * @return Pointer to allocated options or NULL on failure.
 *
 * @note Caller must destroy the returned object via dmlfw_gradient_descent_options_destroy().
 *
 * Usage example in API context:
 * @code
 * dmlfw_gradient_descent_options *opts = dmlfw_gradient_descent_options_create_new();
 * if (opts == NULL || dmlfw_error()) {
 *     // Handle error (e.g., return NULL up the call chain)
 * }
 * // use opts...
 * dmlfw_gradient_descent_options_destroy(opts);
 * @endcode
 *
 * Usage example in main():
 * @code
 * int main() {
 *     char err[512], dbg[512];
 *     dmlfw_gradient_descent_options *opts = dmlfw_gradient_descent_options_create_new();
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to create options: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *     // use opts...
 *     dmlfw_gradient_descent_options_destroy(opts);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_destroy
 */
dmlfw_gradient_descent_options * dmlfw_gradient_descent_options_create_new(void);


/**
 * @brief Destroys a gradient descent options object and frees associated memory.
 *
 * @param gd_options [in] Pointer to options to destroy, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_destroy(opts);
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_create_new
 */
void dmlfw_gradient_descent_options_destroy(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the learning rate.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param learning_rate [in] Learning rate value.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_learning_rate(opts, 0.01);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_learning_rate
 */
void dmlfw_gradient_descent_options_set_learning_rate(dmlfw_gradient_descent_options *gd_options, double learning_rate);


/**
 * @brief Gets the learning rate.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Current learning rate or NaN on error.
 *
 * Usage example:
 * @code
 * double rate = dmlfw_gradient_descent_options_get_learning_rate(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_learning_rate
 */
double dmlfw_gradient_descent_options_get_learning_rate(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the number of iterations.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param number_of_iterations [in] Number of iterations.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_number_of_iterations(opts, 1000);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_number_of_iterations
 */
void dmlfw_gradient_descent_options_set_number_of_iterations(dmlfw_gradient_descent_options *gd_options, uint64_t number_of_iterations);


/**
 * @brief Gets the number of iterations.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Number of iterations or 0 on error.
 *
 * Usage example:
 * @code
 * uint64_t num = dmlfw_gradient_descent_options_get_number_of_iterations(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_number_of_iterations
 */
uint64_t dmlfw_gradient_descent_options_get_number_of_iterations(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the gradient descent type.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param gradient_descent_type [in] One of MLFW_BATCH_GRADIENT_DESCENT,
 *        MLFW_STOCHASTIC_GRADIENT_DESCENT, MLFW_MINI_BATCH_GRADIENT_DESCENT.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_gradient_descent_type(opts, MLFW_BATCH_GRADIENT_DESCENT);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_gradient_descent_type
 */
void dmlfw_gradient_descent_options_set_gradient_descent_type(dmlfw_gradient_descent_options *gd_options, int gradient_descent_type);


/**
 * @brief Gets the gradient descent type.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Gradient descent type or -1 on error.
 *
 * Usage example:
 * @code
 * int type = dmlfw_gradient_descent_options_get_gradient_descent_type(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_gradient_descent_type
 */
int dmlfw_gradient_descent_options_get_gradient_descent_type(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the progress callback.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param progress_callback [in] Callback function pointer or NULL.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_progress_callback(opts, my_callback);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_progress_callback
 */
void dmlfw_gradient_descent_options_set_progress_callback(dmlfw_gradient_descent_options *gd_options, dmlfw_gradient_descent_lin_reg_progress_callback_t progress_callback);


/**
 * @brief Gets the progress callback pointer.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Progress callback function pointer or NULL on error.
 *
 * Usage example:
 * @code
 * auto cb = dmlfw_gradient_descent_options_get_progress_callback(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_progress_callback
 */
dmlfw_gradient_descent_lin_reg_progress_callback_t dmlfw_gradient_descent_options_get_progress_callback(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the data provider callback.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param data_provider [in] Callback function pointer or NULL.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_data_provider(opts, my_data_provider);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_data_provider
 */
void dmlfw_gradient_descent_options_set_data_provider(dmlfw_gradient_descent_options *gd_options, dmlfw_gradient_descent_lin_reg_data_provider_t data_provider);


/**
 * @brief Gets the data provider callback pointer.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Data provider callback function pointer or NULL on error.
 *
 * Usage example:
 * @code
 * auto dp = dmlfw_gradient_descent_options_get_data_provider(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_data_provider
 */
dmlfw_gradient_descent_lin_reg_data_provider_t dmlfw_gradient_descent_options_get_data_provider(dmlfw_gradient_descent_options *gd_options);


/**
 * @brief Sets the mini-batch size for mini-batch gradient descent.
 *
 * @param gd_options [in,out] Gradient descent options (must not be NULL).
 * @param mini_batch_size [in] Number of samples in a mini-batch.
 *
 * Usage example:
 * @code
 * dmlfw_gradient_descent_options_set_mini_batch_size(opts, 32);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_get_mini_batch_size
 */
void dmlfw_gradient_descent_options_set_mini_batch_size(dmlfw_gradient_descent_options *gd_options, uint32_t mini_batch_size);


/**
 * @brief Gets the mini-batch size.
 *
 * @param gd_options [in] Gradient descent options (must not be NULL).
 * @return Mini-batch size or 0 on error.
 *
 * Usage example:
 * @code
 * uint32_t size = dmlfw_gradient_descent_options_get_mini_batch_size(opts);
 * if (dmlfw_error()) {
 *     // Handle error
 * }
 * @endcode
 *
 * @see dmlfw_gradient_descent_options_set_mini_batch_size
 */
uint32_t dmlfw_gradient_descent_options_get_mini_batch_size(dmlfw_gradient_descent_options *gd_options);

/** @} */ ///< End of ml_configurations group
#endif // __DMLFW__ML_CONFIGURATIONS__
