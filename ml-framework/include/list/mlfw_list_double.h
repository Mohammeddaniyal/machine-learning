#ifndef __MLFW__LIST__DOUBLE__
#define __MLFW__LIST__DOUBLE__
#include<mlfw_vector.h>

struct __mlfw_forward_list_double;
typedef struct __mlfw_forward_list_double mlfw_forward_list_double;

mlfw_forward_list_double * mlfw_forward_list_double_create_new();
void mlfw_forward_list_double_destroy(mlfw_forward_list_double *forward_list);

void mlfw_forward_list_double_insert(mlfw_forward_list_double *forward_list,double value);
mlfw_row_vec_string * mlfw_forward_list_double_get_row_vector(mlfw_forward_list_double *forward_list);

dimension_t mlfw_forward_list_double_get_size(mlfw_forward_list_double *forward_list);
void mlfw_forward_list_double_clear(mlfw_forward_list_double *forward_list);

#endif
