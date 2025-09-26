/**
 * @file dmlfw_vector.h
 * @brief Unified interface for vector data structures.
 *
 * This umbrella header consolidates the vector types for
 * double and string elements, providing access to core
 * functionalities such as:
 * - Creation and destruction
 * - Element access and mutation
 * - CSV import/export
 * - Basic manipulation utilities
 *
 * The module supports both row and column vector types for
 * each data specialization, allowing flexible handling of linear arrays.
 * Operations specific to each vector orientation are defined in their respective headers.
 *
 * Each vector type is specialized for its element type,
 * offering tailored operations for numeric or textual data.
 *
 * The vector module forms a foundation for managing linear
 * collections within the framework and is widely reused
 * in higher-level components.
 *
 * @defgroup vector Vector Module
 * @brief Core vector types and utilities for double and string data.
 *
 * See also the specialized headers for detailed APIs:
 * - @ref dmlfw_vec_double.h "Double Vectors"
 * - @ref dmlfw_vec_string.h "String Vectors"
 *
 * @{
 */

#ifndef __DMLFW__VECTOR__
#define __DMLFW__VECTOR__

#include <dmlfw_types.h>            // Core typedefs
#include <vector/dmlfw_vec_string.h>
#include <vector/dmlfw_vec_double.h>

#endif // __DMLFW__VECTOR__

/** @} */ ///< End of vector group
