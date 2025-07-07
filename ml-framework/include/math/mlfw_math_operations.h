#ifndef __MLFW__MATH_OPERATIONS__
#define __MLFW__MATH_OPERATIONS__
#include<mlfw_vector.h>
mlfw_column_vec_double * mlfw_column_vec_double_sigmoid(mlfw_column_vec_double *vector,mlfw_column_vec_double *new_vector);

mlfw_column_vec_double * mlfw_column_vec_double_log(mlfw_column_vec_double *vector,mlfw_column_vec_double *new_vector);

double mlfw_column_vec_double_sum(mlfw_column_vec_double *vector);

mlfw_mat_double * mlfw_mat_double_sigmoid(mlfw_mat_double *matrix,mlfw_mat_double *new_matrix);
#endif
