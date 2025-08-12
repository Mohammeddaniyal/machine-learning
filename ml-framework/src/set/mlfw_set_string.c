#include<mlfw_set.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<___mlfw_error.h>

extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

typedef struct __mlfw_set_string 
{
	char **data;
	dimension_t size;
}mlfw_set_string;
mlfw_set_string * mlfw_set_string_create_new(void)
{
	mlfw_set_string *set;
	set=(mlfw_set_string *)malloc(sizeof(mlfw_set_string));
	if(set==NULL) 
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(mlfw_set_string));
		return NULL;
	}
	set->data=NULL;
	set->size=0;
	return set;
}
void mlfw_set_string_destroy(mlfw_set_string *set)
{
	index_t i;
	if(set==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		return;
	}
	for(i=0;i<set->size;++i)
	{
		free(set->data[i]);
	}
	if(set->data!=NULL) free(set->data);
	free(set);
}

void mlfw_set_string_get(mlfw_set_string *set,index_t i,char **string)
{
	unsigned long int allocation_len;
	if(string==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(set==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		*string=NULL;
		return;
	}
	if(i<0 || i>=set->size)
	{
		if(set->size==0)
		{
			_mlfw_set_error(MLFW_INVALID_INDEX_EMPTY_CODE,MLFW_INVALID_INDEX_EMPTY,i,"i");
		}
		else
		{
			_mlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,i,"i",0,set->size-1);
		}
		*string=NULL;
		return;
	}
	allocation_len=sizeof(char)*(strlen(set->data[i])+1);
	*string=(char *)malloc(allocation_len);
	if(*string==NULL) 
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
	}
	else
	{
	strcpy(*string,set->data[i]);
	}
}
// returns -1 in case of low memory or 0 incase of added or not added because of duplicate
int  mlfw_set_string_add(mlfw_set_string *set,char *string)
{
	unsigned long int allocation_len;
	char *str;
	char **tmp;
	index_t i;
	if(set==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		return -1;
	}
	if(string==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return -1;
	}
	if(set->size==0)
	{
		allocation_len=sizeof(char*)*1;
		set->data=(char **)malloc(allocation_len);
		if(set->data==NULL)
		{
			_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			return -1;
		}
		allocation_len=sizeof(char)*(strlen(string)+1);
		str=(char *)malloc(allocation_len);
		if(str==NULL) 
		{
			_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
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
			_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
			return -1;
		}
		strcpy(str,string);
		allocation_len=sizeof(char *)*(set->size+1);
		tmp=(char **)realloc(set->data,allocation_len);
		if(tmp==NULL)
		{
			_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
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

dimension_t mlfw_set_string_get_size(mlfw_set_string *set)
{
	if(set==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"set");
		return 0;
	}
	return set->size;
}

