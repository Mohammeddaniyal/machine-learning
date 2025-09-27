#ifndef __DMLFW__MATRIX__
#define __DMLFW__MATRIX__
/**
 * @file dmlfw_matrix.h
 * @brief Core matrix types and utilities for double and string data.
 * @ingroup ml-framework
 * @{
 *
 * @author Mohammed Daniyal
 * @version 1.0
 * @date 2025-09-26
 *
 * This umbrella header consolidates matrix types for
 * double and string elements, providing access to core functionalities such as:
 * - Creation and destruction
 * - Element access and mutation
 * - CSV import and export
 * - Core matrix operations and utilities
 *
 * The module supports matrices of different data types and layouts,
 * with operations defined in their respective specialized headers.
 *
 * This modular design facilitates extensibility and reuse across the framework.
 *
 * See also:
 * - @ref dmlfw_mat_double "Double Matrix API"
 * - @ref dmlfw_mat_string "String Matrix API"
 *
 */


#include <dmlfw_types.h>      // core typedefs

#include <dmlfw_vector.h>     // include vector module (row/col vectors)

#include <matrix/dmlfw_mat_double.h>  // double matrix specialized API
#include <matrix/dmlfw_mat_string.h>  // string matrix specialized API

/** @} */

#endif // __DMLFW__MATRIX__
