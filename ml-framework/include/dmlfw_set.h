#ifndef __DMLFW__SET__
#define __DMLFW__SET__
/**
 * @file dmlfw_set.h
 * @defgroup set Unordered Sets
 * @ingroup ml-framework
 * @brief Core unordered set types and utilities for string data.
 *
 * Author: Mohammed Daniyal
 * Version: 1.0
 * Date: 2025-09-26
 *
 * @{
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
 *
 * See also the specialized headers for detailed APIs:
 * - @ref dmlfw_set_string.h "Unordered String Sets"
 *
 */


#include <dmlfw_types.h>             // Core typedefs
#include <set/dmlfw_set_string.h>

/** @} */ ///< End of unordered set group
#endif // __DMLFW__SET__
