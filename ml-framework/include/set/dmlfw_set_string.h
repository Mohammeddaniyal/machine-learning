#ifndef __DMLFW__SET__STRING__
#define __DMLFW__SET__STRING__
/**
 * @file dmlfw_set_string.h
 * @brief Core unordered string set types and utilities.
 * @ingroup set_string
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This header defines opaque unordered set structures specialized for string data.
 * It provides APIs for creation, destruction, element access, adding unique strings,
 * and querying the set size.
 *
 * Error Handling:
 * ----------------
 * All functions report errors via the centralized error API.
 * Use `dmlfw_error()` to check for errors after calls.
 * `dmlfw_get_error_string()` and `dmlfw_get_debug_string()` provide diagnostic details.
 *
 * Ownership:
 * ----------------
 * Functions returning new set pointers transfer ownership to the caller,
 * who is responsible for freeing with `dmlfw_set_string_destroy()`.
 * Returned strings from get operations are allocated and must be freed by callers.
 *
 * @example example_set_string.c
 * Typical usage:
 * @code
 * #include <stdio.h>
 * #include <stdlib.h>
 * #include <dmlfw_set.h>
 * #include <dmlfw_error.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *     dmlfw_set_string *set = dmlfw_set_string_create_new();
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to create set: %s\nDebug info: %s\n", err, dbg);
 *         return EXIT_FAILURE;
 *     }
 *
 *     if (dmlfw_set_string_add(set, "example") == -1 && dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to add string: %s\nDebug info: %s\n", err, dbg);
 *         dmlfw_set_string_destroy(set);
 *         return EXIT_FAILURE;
 *     }
 *
 *     char *retrieved = NULL;
 *     dmlfw_set_string_get(set, 0, &retrieved);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Failed to get string: %s\nDebug info: %s\n", err, dbg);
 *     } else {
 *         printf("First string in set: %s\n", retrieved);
 *         free(retrieved);
 *     }
 *
 *     dimension_t size = dmlfw_set_string_get_size(set);
 *     printf("Set size: %u\n", size);
 *
 *     dmlfw_set_string_destroy(set);
 *     return EXIT_SUCCESS;
 * }
 * @endcode
 */


#include <dmlfw_types.h>
struct __dmlfw_set_string;

/**
 * @brief Opaque structure representing an unordered set of unique strings.
 *
 * The internal details are encapsulated. Use provided API functions
 * for creation, manipulation, and destruction.
 */
typedef struct __dmlfw_set_string dmlfw_set_string;

/**
 * @brief Creates an empty unordered set of strings.
 *
 * @return Pointer to newly created set or NULL if allocation failed.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dmlfw_set_string *set = dmlfw_set_string_create_new();
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to create set: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
dmlfw_set_string *dmlfw_set_string_create_new(void);

/**
 * @brief Destroys the string set, freeing all resources.
 *
 * @param set [in] Pointer to set or NULL (no-op).
 *
 * Usage example:
 * @code
 * dmlfw_set_string_destroy(set);
 * @endcode
 */
void dmlfw_set_string_destroy(dmlfw_set_string *set);

/**
 * @brief Retrieves a copy of the string at the given index.
 *
 * @param set [in] Set (must not be NULL).
 * @param i [in] Zero-based index.
 * @param string [out] Pointer to receive newly allocated string.
 *
 * @note Caller must free the allocated string.
 * @note On error sets error status and string is NULL.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * char *str = NULL;
 * dmlfw_set_string_get(set, 0, &str);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error getting string: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * } else {
 *     printf("String: %s\n", str);
 *     free(str);
 * }
 * @endcode
 */
void dmlfw_set_string_get(dmlfw_set_string *set, index_t i, char **string);

/**
 * @brief Adds a unique string to the set.
 *
 * @param set [in] Set (must not be NULL).
 * @param string [in] String to add (must not be NULL).
 * @return -1 on memory failure,
 *          0 if string was added or already exists.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * int result = dmlfw_set_string_add(set, "example");
 * if (result == -1 && dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Failed to add string: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * @endcode
 */
int dmlfw_set_string_add(dmlfw_set_string *set, char *string);

/**
 * @brief Retrieves the number of unique strings in the set.
 *
 * @param set [in] Set (may be NULL).
 * @return Number of elements or 0 if NULL or error.
 *
 * Usage example:
 * @code
 * char err[512], dbg[512];
 * dimension_t size = dmlfw_set_string_get_size(set);
 * if (dmlfw_error()) {
 *     dmlfw_get_error_string(err, sizeof(err));
 *     dmlfw_get_debug_string(dbg, sizeof(dbg));
 *     printf("Error getting size: %s\nDebug info: %s\n", err, dbg);
 *     return EXIT_FAILURE;
 * }
 * printf("Set size: %u\n", size);
 * @endcode
 */
dimension_t dmlfw_set_string_get_size(dmlfw_set_string *set);
/** @} */ // end of group
#endif /* __DMLFW__SET__STRING__ */
