#include<stdio.h>
#include<stdlib.h>
#include<mlfw_list.h>
#include<mlfw_vector.h>

typedef struct __mlfw_forward_list_node_double
{
	double value;
	struct __mlfw_forward_list_node_double *next;
}mlfw_forward_list_node_double;

typedef struct __mlfw_forward_list_double
{
	mlfw_forward_list_node_double *top;
	dimension_t size;
}mlfw_forward_list_double;

mlfw_forward_list_double * mlfw_forward_list_double_create_new()
{
	mlfw_forward_list_double *forward_list;
	forward_list=(mlfw_forward_list_double *)malloc(sizeof(mlfw_forward_list_double));
	if(forward_list==NULL)
	{
		return NULL;
	}
	forward_list->top=NULL;
	forward_list->size=0;
	return forward_list;
}
void mlfw_forward_list_double_destroy(mlfw_forward_list_double *forward_list)
{
	if(forward_list==NULL) return;
	mlfw_forward_list_double_clear(forward_list);
	free(forward_list);
}

void mlfw_forward_list_double_insert(mlfw_forward_list_double *forward_list,double value)
{
	mlfw_forward_list_node_double *node;
	if(forward_list==NULL) return;
	node=(mlfw_forward_list_node_double *)malloc(sizeof(mlfw_forward_list_node_double));
	if(node==NULL) return;
	node->value=value;
	node->next=forward_list->top;
	forward_list->top=node;
	forward_list->size++;
}
mlfw_row_vec_double * mlfw_forward_list_double_get_row_vector(mlfw_forward_list_double *forward_list)
{
	mlfw_row_vec_double *vector;
	mlfw_forward_list_node_double *node;
	index_t i;
	if(forward_list==NULL) return NULL;
	if(forward_list->size==0) return NULL;
	vector=mlfw_row_vec_double_create_new(forward_list->size);
	if(vector==NULL) return NULL;
	for(node=forward_list->top,i=0;node!=NULL;node=node->next,++i)
	{
		mlfw_row_vec_double_set(vector,i,node->value);
	}
	return vector;
}

dimension_t mlfw_forward_list_double_get_size(mlfw_forward_list_double *forward_list)
{
	if(forward_list==NULL) return 0;
	return forward_list->size;
}
void mlfw_forward_list_double_clear(mlfw_forward_list_double *forward_list)
{
	mlfw_forward_list_node_double *node;
	if(forward_list==NULL) return;
	while(forward_list->top!=NULL)
	{
		node=forward_list->top;
		forward_list->top=forward->top->next;
		free(node);
		forward_list->size--; // also we can directly assign zero
	}
}

