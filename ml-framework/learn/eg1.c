#include<stdio.h>
#include<inttypes.h>

#define MLFW_LOW_MEMORY_CODE 1
#define MLFW_LOW_MEMORY "Insufficient memory, cannot allocate %u bytes"

#define MLFW_NULL_ARGUMENT_CODE 2
#define MLFW_NULL_ARGUMENT "null argument against %s"

#define MLFW_INVALID_INDEX_CODE 3
#define MLFW_INVALID_INDEX "Invalid index (%u) against (%s), valid range (%u-%u)"

#define _mlfw_set_error(code,string,...) \
	mlfw_error_code=code; \
	snprintf(mlfw_error_string,511,"File : %s\nFunction : %s\nLine : %d",__FILE__,__FUNCTION__,__LINE__);

