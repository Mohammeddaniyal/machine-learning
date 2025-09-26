/**
 * @file dmlfw_encoder.h
 * @brief Unified interface for data encoding operations.
 *
 * This umbrella header consolidates the encoder modules for 
 * categorical and binary feature encoding, providing access to core
 * data transformation functionalities such as:
 * - One-hot encoding
 * - Binary encoding
 * - Custom encoding algorithms
 *
 * The encoder module enables conversion of textual or categorical column data
 * into encoded numerical representations suitable for machine learning and data processing tasks.
 * Specialized encoder operations are defined in their respective headers.
 *
 * These encoding tools are foundational for feature engineering, model training,
 * and interoperability with common ML workflows.
 *
 * @defgroup encoder Encoder Module
 * @brief Core data encoding and transformation utilities.
 *
 * See also the specialized headers for detailed APIs:
 * - @ref dmlfw_data_encoder.h "Data Encoder API"
 *
 * @{
 */

#ifndef __DMLFW__ENCODER__
#define __DMLFW__ENCODER__

#include <dmlfw_types.h>          // Core typedefs
#include <encoder/dmlfw_data_encoder.h>

#endif // __DMLFW__ENCODER__

/** @} */ ///< End of encoder group
