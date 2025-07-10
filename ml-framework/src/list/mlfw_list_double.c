#include<stdio.h>
#include<stdlib.h>
#include<mlfw_list.h>
#include<mlfw_vector.h>

typedef struct __mlfw_forward_list_node_double
{
	double value;
	__mlfw_forward_list_node_double *next;
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
	forward_list->next=NULL;
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
	node=(mlfw_forward_list_node_double)malloc(sizeof(mlfw_forward_list_node_double));
	if(node==NULL) return;
	node->value=value;
	node->next=forward_list->top;
	forward_list->top=node;
	forward_list->size++;
}
mlfw_row_vec_string * mlfw_forward_list_double_get_row_vector(mlfw_forward_list_double *forward_list)
{
}

dimension_t mlfw_forward_list_double_get_size(mlfw_forward_list_double *forward_list)
{
}
void mlfw_forward_list_double_clear(mlfw_forward_list_double *forward_list)
{
}

