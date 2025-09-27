#ifndef __DMLFW__ENCODER__
#define __DMLFW__ENCODER__
/**
 * @file dmlfw_encoder.h
 * @brief Core data encoding and transformation utilities.
 * @ingroup ml-framework
 * @{
 *
 * @author Mohammed Daniyal
 * @version 1.0
 * @date 2025-09-26
 *
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
 *
 * See also the specialized headers for detailed APIs:
 * - @ref dmlfw_data_encoder.h "Data Encoder API"
 *
 */

#include <dmlfw_types.h>          // Core typedefs
#include <encoder/dmlfw_data_encoder.h>

/** @} */ ///< End of encoder group
#endif // __DMLFW__ENCODER__

