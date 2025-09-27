#ifndef __DMLFW__TYPES__
#define __DMLFW__TYPES__
/**
 * @file dmlfw_types.h
 * @brief Fundamental scalar typedefs used across the framework.
 * @ingroup ml-framework
 * @{
 *
 * @author Mohammed Daniyal
 * @version 1.0
 * @date 2025-09-26
 *
 *
 * This header centralizes typedefs for indices, dimensions,
 * and other fundamental scalar types. All modules should
 * include this instead of redefining these types.
 */


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


/** @} */ // end of core_types group
#endif // __DMLFW__TYPES__