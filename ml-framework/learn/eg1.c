#include<stdio.h>
#include<inttypes.h>


uint32_t dmlfw_error_code;
char dmlfw_error_string[512];
char dmlfw_error_debug_info[512];

#define MLFW_LOW_MEMORY_CODE 1
#define MLFW_LOW_MEMORY "Insufficient memory, cannot allocate %u bytes"

#define MLFW_NULL_ARGUMENT_CODE 2
#define MLFW_NULL_ARGUMENT "null argument against %s"

#define MLFW_INVALID_INDEX_CODE 3
#define MLFW_INVALID_INDEX "Invalid index (%u) against (%s), valid range (%u-%u)"

#define MLFW_UNKNOWN_ERROR_CODE 4
#define MLFW_UNKNOWN_ERROR "Unknown error"

#define _dmlfw_set_error(code,string,...) \
	dmlfw_error_code=code; \
	snprintf(dmlfw_error_string,511,string,## __VA_ARGS__); \
	snprintf(dmlfw_error_debug_info,511,"File : %s\nFunction : %s\nLine : %d",__FILE__,__FUNCTION__,__LINE__);
// ## reason M4 L8 (27:16) 
int main()
{
	//setup error for low memory
	_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,342321);
	printf("Error code : %u\n",dmlfw_error_code);
	printf("Error : %s\n",dmlfw_error_string);
	printf("Debug info : %s\n",dmlfw_error_debug_info);	
	//setup error for NULL ARGUMENT
	_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"ptr");
	printf("Error code : %u\n",dmlfw_error_code);
	printf("Error : %s\n",dmlfw_error_string);
	printf("Debug info : %s\n",dmlfw_error_debug_info);	
	//setup error for INVALID_INDEX
	_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,5,"start_index",0,2);
	printf("Error code : %u\n",dmlfw_error_code);
	printf("Error : %s\n",dmlfw_error_string);
	printf("Debug info : %s\n",dmlfw_error_debug_info);	
	//setup error for UNKNOWN ERROR
	_dmlfw_set_error(MLFW_UNKNOWN_ERROR_CODE,MLFW_UNKNOWN_ERROR);
	printf("Error code : %u\n",dmlfw_error_code);
	printf("Error : %s\n",dmlfw_error_string);
	printf("Debug info : %s\n",dmlfw_error_debug_info);	
	return 0;
}
