#ifndef __MLFW__LOGISTIC__REGRESSION__
#define __MLFW__LOGISTIC__REGRESSION__
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_set.h>
#include<inttypes.h>

mlfw_row_vec_double * mlfw_logistic_regression_gradient_descent_fit(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost));

mlfw_column_vec_double * mlfw_logistic_regression_predict(mlfw_mat_double *input_features_matrix,mlfw_row_vec_double *trained_parameters);

mlfw_mat_double * mlfw_logistic_regression_gradient_descent_multi_class_fit(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,mlfw_set_string *class_set,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost));


mlfw_column_vec_double * mlfw_logistic_regression_multi_class_predict(mlfw_mat_double *input_features_matrix,mlfw_set_string *class_set,mlfw_mat_double *trained_parameters_matrix);
#endif
