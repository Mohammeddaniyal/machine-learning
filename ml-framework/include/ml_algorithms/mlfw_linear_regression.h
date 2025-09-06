#ifndef __MLFW__LINEAR_REGRESSION__
#define __MLFW__LINEAR_REGRESSION__
#include<mlfw_vector.h>
#include<mlfw_ml_configurations.h>
#include<mlfw_vector.h>

mlfw_column_vec_double * mlfw_linear_regression_fit_using_batch_gradient_descent(mlfw_gradient_descent_options  *gd_options,mlfw_mat_double *x,mlfw_column_vec_double *y,double regularization_parameter,mlfw_column_vec_double *model);

mlfw_column_vec_double * mlfw_linear_regression_predict(mlfw_mat_double *x,mlfw_column_vec_double *model);
#endif
