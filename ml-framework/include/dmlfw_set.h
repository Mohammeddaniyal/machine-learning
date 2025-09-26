/**
 * @file dmlfw_set.h
 * @brief Unified interface for unordered set data structures.
 *
 * This umbrella header consolidates the unordered set types, providing access 
 * to core functionalities such as:
 * - Creation and destruction
 * - Element access and mutation
 *
 * The module currently supports unordered string set types, with operations defined 
 * in their respective headers.
 *
 * The unordered set module is designed for managing collections of unique elements 
 * without inherent ordering within the framework and is widely reused in higher-level components.
 *
 * @defgroup set Unordered Set Module
 * @brief Core unordered set types and utilities for string data.
 *
 * See also the specialized headers for detailed APIs:
 * - @ref dmlfw_set_string.h "Unordered String Sets"
 *
 * @{
 */

#ifndef __DMLFW__SET__
#define __DMLFW__SET__

#include <dmlfw_types.h>             // Core typedefs
#include <set/dmlfw_set_string.h>

#endif // __DMLFW__SET__

/** @} */ ///< End of unordered set group
