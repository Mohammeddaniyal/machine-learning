#ifndef __DMLFW__MATH_OPERATIONS__
#define __DMLFW__MATH_OPERATIONS__
#include<dmlfw_vector.h>
#include<dmlfw_matrix.h>
dmlfw_column_vec_double * dmlfw_column_vec_double_sigmoid(dmlfw_column_vec_double *vector,dmlfw_column_vec_double *new_vector);

dmlfw_column_vec_double * dmlfw_column_vec_double_log(dmlfw_column_vec_double *vector,dmlfw_column_vec_double *new_vector);

double dmlfw_column_vec_double_sum(dmlfw_column_vec_double *vector);

dmlfw_mat_double * dmlfw_mat_double_sigmoid(dmlfw_mat_double *matrix,dmlfw_mat_double *new_matrix);
#endif
