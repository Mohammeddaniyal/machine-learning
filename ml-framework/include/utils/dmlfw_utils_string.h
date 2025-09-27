#ifndef __DMLFW__UTILS__STRING__
#define __DMLFW__UTILS__STRING__
/**
 * @file dmlfw_utils_string.h
 * @brief Common string-related utility functions.
 * @ingroup utils_string
 * @{
 *
 * @version 1.0
 * @date 2025-09-25
 *
 * This header provides essential string operations:
 * - Case-insensitive string comparison
 * - Conversion of uint32_t integer to its binary string representation
 *
 * Error Handling:
 * ---------------
 * Functions use the centralized error API to report failures.
 * Use `dmlfw_error()` after calls to check status,
 * and `dmlfw_get_error_string()` plus `dmlfw_get_debug_string()` for diagnostics.
 *
 * @example example_utils_string.c
 * Typical usage:
 * @code
 * #include <stdio.h>
 * #include <string.h>
 * #include <dmlfw_utils.h>
 *
 * int main(void) {
 *     char err[512], dbg[512];
 *
 *     const char *s1 = "Hello";
 *     const char *s2 = "hello";
 *
 *     int cmp = dmlfw_strcmp_case_insensitive(s1, s2);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Comparison error: %s\nDebug info: %s\n", err, dbg);
 *         return 1;
 *     }
 *     printf("Comparison result: %d\n", cmp);
 *
 *     char binary_str[33]; // 32 bits + null
 *     dmlfw_uint32_to_binary(12345U, binary_str);
 *     if (dmlfw_error()) {
 *         dmlfw_get_error_string(err, sizeof(err));
 *         dmlfw_get_debug_string(dbg, sizeof(dbg));
 *         printf("Conversion error: %s\nDebug info: %s\n", err, dbg);
 *         return 1;
 *     }
 *     printf("Binary representation: %s\n", binary_str);
 *
 *     return 0;
 * }
 * @endcode
 */

/**
 * @brief Compares two strings ignoring case differences.
 *
 * Performs a case-insensitive comparison of `left` and `right`.
 *
 * @param left [in] Left string (must not be NULL).
 * @param right [in] Right string (must not be NULL).
 * @return 0 if strings are equal ignoring case, negative if left < right,
 *         positive if left > right.
 */
int dmlfw_strcmp_case_insensitive(const char *left, const char *right);


/**
 * @brief Converts a 32-bit unsigned integer to a binary string.
 *
 * Converts `number` to a null-terminated string of '0' and '1' characters,
 * representing the binary format from most significant bit to least.
 *
 * @param number [in] Number to convert.
 * @param string [out] Destination buffer to hold binary string.
 *                   Must be at least 33 bytes in size (32 bits + null).
 */
void dmlfw_uint32_to_binary(uint32_t number, char *string);

#endif // __DMLFW__UTILS__STRING__
/** @} */ ///< End of utils_string group