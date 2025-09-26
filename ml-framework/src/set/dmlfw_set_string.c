#include<dmlfw_set.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

typedef struct __dmlfw_set_string 
{
	char **data;
	dimension_t size;
}dmlfw_set_string;
dmlfw_set_string * dmlfw_set_string_create_new(void)
{
	dmlfw_set_string *set;
	dmlfw_reset_error();
	set=(dmlfw_set_string *)malloc(sizeof(dmlfw_set_string));
	if(set==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_set_string));
		return NULL;
	}
	set->data=NULL;
	set->size=0;
	return set;
}
void dmlfw_set_string_destroy(dmlfw_set_string *set)
{
	index_t i;
	dmlfw_reset_error();
	if(set==NULL) 
	{
		return;
	}
	for(i=0;i<set->size;++i)
	{
		free(set->data[i]);
	}
	if(set->data!=NULL) free(set->data);
	free(set);
}

void dmlfw_set_string_get(dmlfw_set_string *set,index_t i,char **string)
{
	unsigned long int allocation_len;
	dmlfw_reset_error();
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(set==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		*string=NULL;
		return;
	}
	if(i<0 || i>=set->size)
	{
		if(set->size==0)
		{
			_dmlfw_set_error(MLFW_INVALID_INDEX_EMPTY_CODE,MLFW_INVALID_INDEX_EMPTY,i,"i");
		}
		else
		{
			_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,i,"i",0,set->size-1);
		}
		*string=NULL;
		return;
	}
	allocation_len=sizeof(char)*(strlen(set->data[i])+1);
	*string=(char *)malloc(allocation_len);
	if(*string==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
	}
	else
	{
	strcpy(*string,set->data[i]);
	}
}
// returns -1 in case of low memory or 0 incase of added or not added because of duplicate
int  dmlfw_set_string_add(dmlfw_set_string *set,char *string)
{
	unsigned long int allocation_len;
	char *str;
	char **tmp;
	index_t i;
	dmlfw_reset_error();
	if(set==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		return -1;
	}
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return -1;
	}
	if(set->size==0)
	{
		allocation_len=sizeof(char*)*1;
		set->data=(char **)malloc(allocation_len);
		if(set->data==NULL)
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			return -1;
		}
		allocation_len=sizeof(char)*(strlen(string)+1);
		str=(char *)malloc(allocation_len);
		if(str==NULL) 
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			free(set->data);
			set->data=NULL;
			return -1;
		}
		strcpy(str,string);
		set->data[0]=str;
		set->size++;
	}
	else
	{
		for(i=0;i<set->size;++i)
		{
			if(strcmp(set->data[i],string)==0)
				{
					return 0;
				}
		}
				
		allocation_len=sizeof(char)*(strlen(string)+1);
		str=(char *)malloc(allocation_len);
		if(str==NULL) 
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			return -1;
		}
		strcpy(str,string);
		allocation_len=sizeof(char *)*(set->size+1);
		tmp=(char **)realloc(set->data,allocation_len);
		if(tmp==NULL)
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			free(str);
			return -1;
		}
		set->data=tmp;
		set->data[set->size]=str;
		set->size++;
		return 0;
	}
	return 0;
}

dimension_t dmlfw_set_string_get_size(dmlfw_set_string *set)
{
	dmlfw_reset_error();
	if(set==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		return 0;
	}
	return set->size;
}

