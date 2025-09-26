#include<stdio.h>
#include<stdlib.h>
#include<dmlfw_list.h>
#include<dmlfw_vector.h>
#include<___dmlfw_error.h>
#include<dmlfw_error.h>
extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];


typedef struct __dmlfw_forward_list_node_double
{
	double value;
	struct __dmlfw_forward_list_node_double *next;
}dmlfw_forward_list_node_double;

typedef struct __dmlfw_forward_list_double
{
	dmlfw_forward_list_node_double *top;
	dimension_t size;
}dmlfw_forward_list_double;

dmlfw_forward_list_double * dmlfw_forward_list_double_create_new()
{
	dmlfw_forward_list_double *forward_list;
	dmlfw_reset_error();
	forward_list=(dmlfw_forward_list_double *)malloc(sizeof(dmlfw_forward_list_double));
	if(forward_list==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_forward_list_double));
		return NULL;
	}
	forward_list->top=NULL;
	forward_list->size=0;
	return forward_list;
}
void dmlfw_forward_list_double_destroy(dmlfw_forward_list_double *forward_list)
{
	dmlfw_reset_error();
	if(forward_list==NULL)
	{
		return;
	}
	dmlfw_forward_list_double_clear(forward_list);
	free(forward_list);
}

void dmlfw_forward_list_double_insert(dmlfw_forward_list_double *forward_list,double value)
{
	dmlfw_forward_list_node_double *node;
	dmlfw_reset_error();
	if(forward_list==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"forward_list");
		return;
	}
	node=(dmlfw_forward_list_node_double *)malloc(sizeof(dmlfw_forward_list_node_double));
	if(node==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_forward_list_node_double));
		return;
	}
	node->value=value;
	node->next=forward_list->top;
	forward_list->top=node;
	forward_list->size++;
}
dmlfw_row_vec_double * dmlfw_forward_list_double_get_row_vector(dmlfw_forward_list_double *forward_list)
{
	dmlfw_row_vec_double *vector;
	dmlfw_forward_list_node_double *node;
	index_t i;
	dmlfw_reset_error();
	if(forward_list==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"forward_list");
		return NULL;
	}
	if(forward_list->size==0) 
	{
		_dmlfw_set_error(MLFW_LIST_EMPTY_CODE,MLFW_LIST_EMPTY,"forward_list");
		return NULL;
	}
	vector=dmlfw_row_vec_double_create_new(forward_list->size);
	if(dmlfw_error()) return NULL;
	for(node=forward_list->top,i=0;node!=NULL;node=node->next,++i)
	{
		dmlfw_row_vec_double_set(vector,i,node->value);
	}
	return vector;
}

dimension_t dmlfw_forward_list_double_get_size(dmlfw_forward_list_double *forward_list)
{
	dmlfw_reset_error();
	if(forward_list==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"forward_list");
		return 0;
	}
	return forward_list->size;
}
void dmlfw_forward_list_double_clear(dmlfw_forward_list_double *forward_list)
{
	dmlfw_forward_list_node_double *node;
	dmlfw_reset_error();
	if(forward_list==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"forward_list");
		return;
	}
	while(forward_list->top!=NULL)
	{
		node=forward_list->top;
		forward_list->top=forward_list->top->next;
		free(node);
		forward_list->size--; // also we can directly assign zero
	}
}

