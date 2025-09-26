/**
 * @file dmlfw_types.h
 * @brief Core type definitions used across the framework.
 *
 * This header centralizes typedefs for indices, dimensions,
 * and other fundamental scalar types. All modules should
 * include this instead of redefining these types.
 */

#ifndef __DMLFW__TYPES__
#define __DMLFW__TYPES__

#include <inttypes.h>

/**
 * @typedef dimension_t
 * @brief Represents the size or dimension of a data structure (uint32_t).
 */
typedef uint32_t dimension_t;

/**
 * @typedef index_t
 * @brief Represents an index within a data structure (uint32_t).
 */
typedef uint32_t index_t;

#endif // __DMLFW__TYPES__
