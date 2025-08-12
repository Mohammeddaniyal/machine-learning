#include<string.h>
#include<inttypes.h>
#include<___mlfw_error.h>

__thread uint32_t _mlfw_error_code;
__thread char _mlfw_error_string[512];
__thread char _mlfw_debug_string[512];


uint8_t mlfw_error(void)
{
	return _mlfw_error_code!=0; // !=0 means, error exists
}
uint32_t mlfw_get_error_code(void)
{
	return 0;
}
void mlfw_get_error_string(char *error_string,uint8_t size)
{
	uint32_t i;
	uint32_t max_len;
	char *p;
	char *q;
	if(error_string==null) return;
	if(size==0) return;
	if(_mlfw_error_code=0)
	{
		error_string[0]='\0';
		return;
	}
	max_len=size-1;
	i=0;
	p=_mlfw_error_string;
	q=error_string;
	while(*p!='\0' && i<max_len)
	{
		*q=*p;
		++p;
		++q;
		++i;
	}
	*q='\0';
}
void mlfw_get_debug_string(char *debug_string,uint_t size)
{
	uint32_t i;
	uint32_t max_len;
	char *p;
	char *q;
	if(debug_string==null) return;
	if(size==0) return;
	if(_mlfw_error_code=0)
	{
		debug_string[0]='\0';
		return;
	}
	max_len=size-1;
	i=0;
	p=_mlfw_debug_string;
	q=debug_string;
	while(*p!='\0' && i<max_len)
	{
		*q=*p;
		++p;
		++q;
		++i;
	}
	*q='\0';

}
void mlfw_reset_error(void)
{
	_mlfw_error_code=0;
	_mlfw_error_string[0]='\0';
	_mlfw_debug_string[0]='\0';
}
uint32_t mlfw_get_error_string_length(void)
{
	return (uint32_t)strlen(_mlfw_error_string);
}
uint32_t mlfw_get_debug_string_lenght(void)
{
	return (uint32_t)strlen(_mlfw_error_string);
}
