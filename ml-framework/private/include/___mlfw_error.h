#ifndef __PRIVATE_MLFW__ERROR__
#define __PRIVATE_MLFW__ERROR__
#include<stdio.h>
#include<inttypes.h>


__thread uint32_t _mlfw_error_code;
__thread char _mlfw_error_string[512];
__thread char _mlfw_debug_string[512];

#define MLFW_LOW_MEMORY_CODE 1
#define MLFW_LOW_MEMORY "Insufficient memory, cannot allocate (%lu) bytes"

#define MLFW_NULL_ARGUMENT_CODE 2
#define MLFW_NULL_ARGUMENT "null argument against (%s)"

#define MLFW_INVALID_INDEX_CODE 3
#define MLFW_INVALID_INDEX "Invalid index (%u) against (%s), valid range (%u-%u)"

#define MLFW_INVALID_INDEX_EMPTY_CODE 4
#define MLFW_INVALID_INDEX_EMPTY "Invalid index (%u) for parameter (%s). collection is empty"

void mlfw_reset_error();

#define _mlfw_set_error(code,string,...) \
	_mlfw_error_code=code; \
	snprintf(_mlfw_error_string,511,string,## __VA_ARGS__); \
	snprintf(_mlfw_debug_string,511,"File : %s\nFunction : %s\nLine : %d",__FILE__,__FUNCTION__,__LINE__);

#endif
