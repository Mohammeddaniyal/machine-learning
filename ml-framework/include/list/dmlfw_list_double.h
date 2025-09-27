#ifndef __DMLFW__LIST__DOUBLE__
#define __DMLFW__LIST__DOUBLE__
/**
 * @file dmlfw_list_double.h
 * @defgroup list_double Forward List (Double)
 * @ingroup list
 * @brief Core singly linked list type for double precision data.
 *
 * Version: 1.0
 * Date: 2025-09-25
 *
 * @{ 
 *
 * This module provides a singly linked list (forward list) implementation 
 * for double precision floating point data. It supports creating, 
 * destroying, inserting elements, converting the list to a row vector, 
 * querying size, and clearing elements.
 *
 * Error Handling:
 * ----------------
 * All functions report errors through a centralized error API.
 * Use `dmlfw_error()` to check for errors after each call.
 * `dmlfw_get_error_string()` and `dmlfw_get_debug_string()` provide diagnostic messages.
 *
 * Ownership:
 * ----------------
 * The list owns its nodes and their double values.
 * The vector returned by conversion functions must be explicitly freed by callers.
 *
 *
 * @example example_dmlfw_list_double.c
 * Example usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_list.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_forward_list_double *list = dmlfw_forward_list_double_create_new();
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to create forward list: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     dmlfw_forward_list_double_insert(list, 3.14);
 *     dmlfw_forward_list_double_insert(list, 2.71);
 *
 *     dimension_t size = dmlfw_forward_list_double_get_size(list);
 *     printf("List size: %u\n", size);
 *
 *     dmlfw_row_vec_double *vec = dmlfw_forward_list_double_get_row_vector(list);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to convert list to vector: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_forward_list_double_destroy(list);
 *         return EXIT_FAILURE;
 *     }
 *
 *     double val = dmlfw_row_vec_double_get(vec, 0);
 *     printf("First element in vector: %lf\n", val);
 *
 *     dmlfw_row_vec_double_destroy(vec);
 *     dmlfw_forward_list_double_clear(list);
 *     dmlfw_forward_list_double_destroy(list);
 *
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */


#include <dmlfw_vector.h>

struct __dmlfw_forward_list_double;

/**
 * @brief Opaque structure representing a forward list of double values.
 *
 * The internal details are hidden to enforce encapsulation.
 * Use the provided API functions to create, manipulate, and destroy instances.
 */
typedef struct __dmlfw_forward_list_double dmlfw_forward_list_double;


/**
 * @brief Creates a new empty forward list of doubles.
 *
 * @return Pointer to newly allocated empty list or NULL on failure.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_forward_list_double *list = dmlfw_forward_list_double_create_new();
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to create forward list: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_forward_list_double *dmlfw_forward_list_double_create_new(void);

/**
 * @brief Destroys the forward list and frees all memory.
 *
 * @param forward_list [in] Pointer to the list to destroy, or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_forward_list_double_destroy(list);
 * @endcode
 */
void dmlfw_forward_list_double_destroy(dmlfw_forward_list_double *forward_list);

/**
 * @brief Inserts a double value at the front of the list.
 *
 * @param forward_list [in,out] List to insert into (non-NULL).
 * @param value [in] Value to insert.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_forward_list_double_insert(list, 4.56);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to insert value: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_forward_list_double_insert(dmlfw_forward_list_double *forward_list, double value);

/**
 * @brief Converts the forward list into a row vector of doubles.
 *
 * @param forward_list [in] List to convert (non-NULL).
 * @return Pointer to newly allocated row vector or NULL on error.
 *         Caller is responsible for destroying the returned vector.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_row_vec_double *vec = dmlfw_forward_list_double_get_row_vector(list);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to convert list to vector: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * // use vec...
 * dmlfw_row_vec_double_destroy(vec);
 * @endcode
 */
dmlfw_row_vec_double *dmlfw_forward_list_double_get_row_vector(dmlfw_forward_list_double *forward_list);

/**
 * @brief Returns the number of elements in the list.
 *
 * @param forward_list [in] List to query.
 * @return Number of elements or 0 if list is NULL.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dimension_t size = dmlfw_forward_list_double_get_size(list);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to get list size: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * printf("List size: %u\n", size);
 * @endcode
 */
dimension_t dmlfw_forward_list_double_get_size(dmlfw_forward_list_double *forward_list);

/**
 * @brief Removes all elements from the list.
 *
 * @param forward_list [in,out] List to clear (non-NULL).
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_forward_list_double_clear(list);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to clear list: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
void dmlfw_forward_list_double_clear(dmlfw_forward_list_double *forward_list);

/** @} */
#endif // __DMLFW__LIST__DOUBLE__

