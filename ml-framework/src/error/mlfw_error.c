#include<string.h>
#include<inttypes.h>
#include<___mlfw_error.h>

uint8_t mlfw_error()
{
	return _mlfw_error_code!=0; // !=0 means, error exists
}
uint32_t mlfw_get_error_code()
{
	return 0;
}
void mlfw_get_error_string(char *error_string,uint8_t size)
{
}
void mlfw_get_debug_string(char *debig_string,uint_t size)
{
}
void mlfw_reset_error()
{
	_mlfw_error_code=0;
	_mlfw_error_string[0]='\0';
	_mlfw_debug_string[0]='\0';
}
uint32_t mlfw_get_error_string_length()
{
}
uint32_t mlfw_get_debug_string_lenght()
{
}
