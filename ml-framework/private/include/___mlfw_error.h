#ifndef __PRIVATE_MLFW__ERROR__
#define __PRIVATE_MLFW__ERROR__
#include<stdio.h>
#include<inttypes.h>


#define MLFW_LOW_MEMORY_CODE 1
#define MLFW_LOW_MEMORY "Insufficient memory, cannot allocate (%lu) bytes"

#define MLFW_NULL_ARGUMENT_CODE 2
#define MLFW_NULL_ARGUMENT "null argument against (%s)"

#define MLFW_INVALID_INDEX_CODE 3
#define MLFW_INVALID_INDEX "Invalid index (%u) against (%s), valid range (%u-%u)"

#define MLFW_INVALID_INDEX_EMPTY_CODE 4
#define MLFW_INVALID_INDEX_EMPTY "Invalid index (%u) for parameter (%s). collection is empty"

#define MLFW_INVALID_VECTOR_SIZE_CODE 5
#define MLFW_INVALID_VECTOR_SIZE "Invalid vector size (%u)"

#define MLFW_INVALID_CONTAINER_SIZE_CODE 6
#define MLFW_INVALID_CONTAINER_SIZE "Size of container (%s)  is (%u), whereas value of parameter (%s) is (%u)"

#define MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE_CODE 7
#define MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE "Size of vector header parameter (%s) is (%u) whereas (%u) is required"

#define MLFW_UNABLE_TO_CREATE_FILE_CODE 8
#define MLFW_UNABLE_TO_CREATE_FILE "Unable to create file named (%s) specified against parameter (%s)"

#define MLFW_UNABLE_TO_OPEN_FILE_CODE 9 
#define MLFW_UNABLE_TO_OPEN_FILE "Unable to open file named (%s) specified against parameter (%s)"

#define MLFW_INVALID_HEADER_SIZE_IN_FILE_CODE 10
#define MLFW_INVALID_HEADER_SIZE_IN_FILE "Header size in file (%s) is (%u), required (%u)"


#define MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE 11
#define MLFW_INVALID_ROW_VECTOR_HEADER_SIZE "Size of vector header parameter (%s) is (%u), whereas (%u) is required"

#define MLFW_NO_STRING_SET_CODE 12
#define MLFW_NO_STRING_SET "String not set against index (%u)"

#define MLFW_INVALID_GRADIENT_DESCENT_TYPE_CODE 13
#define MLFW_INVALID_GRADIENT_DESCENT_TYPE "Invalid gradient descent type, expected BATCH, STOCHASTIC or MINI BATCH, refer documentation"

#define MLFW_LIST_EMPTY_CODE 14
#define MLFW_LIST_EMPTY "Forward list represented by (%s) is empty"

#define MLFW_VECTOR_SIZE_NOT_SAME_CODE 15
#define MLFW_VECTOR_SIZE_NOT_SAME "Size of vectors (%s) and (%s) not same, (%u) and (%u), hence operation is not possible"

#define MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT_CODE 16
#define MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT "Size of parameter (%s) is (%u) whereas (%u) is required"

#define MLFW_INVALID_RIGHT_OPERAND_VECTOR_SIZE_CODE 17
#define MLFW_INVALID_RIGHT_OPERAND_VECTOR_SIZE "Invalid size of right operand parameter vector (%s), size is (%u) and expected size is (%u)"

#define MLFW_INVALID_RIGHT_OPERAND_MATRIX_ROWS_COUNT_CODE 18
#define MLFW_INVALID_RIGHT_OPERAND_MATRIX_ROWS_COUNT "Invalid number of rows of right operand parameter matrix (%s), row count is (%u) and expected row count is (%u)"

#define MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE 19
#define MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT "Dimensions of parameter (%s) are (%u) rows and (%u) columns whereas required dimensions are (%u) rows and (%u) columns"

#define MLFW_INVALID_RIGHT_OPERAND_MATRIX_DIMENSIONS_CODE 20
#define MLFW_INVALID_RIGHT_OPERAND_MATRIX_DIMENSIONS "Dimensions of parameter (%s) are (%u) rows and (%u) columns whereas required dimensions are (%u) rows and (%u) columns"

#define MLFW_NOT_SQUARE_MATRIX_CODE 21
#define MLFW_NOT_SQUARE_MATRIX "Parameter (%s) is not a square matrix"

#define MLFW_SQUARE_MATRIX_DIMENSIONS_SHOULD_BE_POWER_OF_TWO_CODE 22
#define MLFW_SQUARE_MATRIX_DIMENSIONS_SHOULD_BE_POWER_OF_TWO "Dimensions of parameter (%s) are (%u) rows and (%u) columns, but in power of two required for strassens algorithm"

#define MLFW_INVALID_MODEL_VECTOR_SIZE_CODE 23
#define MLFW_INVALID_MODEL_VECTOR_SIZE "Size of parameter (%s) is (%u) whereas (%u) is expected"

void mlfw_reset_error();

#define _mlfw_set_error(code,string,...) \
	_mlfw_error_code=code; \
	snprintf(_mlfw_error_string,511,string,## __VA_ARGS__); \
	snprintf(_mlfw_debug_string,511,"File : %s\nFunction : %s\nLine : %d",__FILE__,__FUNCTION__,__LINE__);

#endif
