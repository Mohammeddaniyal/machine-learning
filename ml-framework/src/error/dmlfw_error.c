#include<string.h>
#include<inttypes.h>
#include<___dmlfw_error.h>
#include<dmlfw_error.h>
__thread uint32_t _dmlfw_error_code;
__thread char _dmlfw_error_string[512];
__thread char _dmlfw_debug_string[512];


uint8_t dmlfw_error(void)
{
	return _dmlfw_error_code!=0; // !=0 means, error exists
}
uint32_t dmlfw_get_error_code(void)
{
	return _dmlfw_error_code;
}
void dmlfw_get_error_string(char *error_string,uint32_t size)
{
	uint32_t i;
	uint32_t max_len;
	char *p;
	char *q;
	if(error_string==NULL) return;
	if(size==0) return;
	if(_dmlfw_error_code==0)
	{
		error_string[0]='\0';
		return;
	}
	max_len=size-1;
	i=0;
	p=_dmlfw_error_string;
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
void dmlfw_get_debug_string(char *debug_string,uint32_t size)
{
	uint32_t i;
	uint32_t max_len;
	char *p;
	char *q;
	if(debug_string==NULL) return;
	if(size==0) return;
	if(_dmlfw_error_code==0)
	{
		debug_string[0]='\0';
		return;
	}
	max_len=size-1;
	i=0;
	p=_dmlfw_debug_string;
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
void dmlfw_reset_error(void)
{
	_dmlfw_error_code=0;
	_dmlfw_error_string[0]='\0';
	_dmlfw_debug_string[0]='\0';
}
uint32_t dmlfw_get_error_string_length(void)
{
	return (uint32_t)strlen(_dmlfw_error_string);
}
uint32_t dmlfw_get_debug_string_length(void)
{
	return (uint32_t)strlen(_dmlfw_error_string);
}
