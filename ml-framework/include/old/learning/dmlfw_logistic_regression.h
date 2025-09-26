#ifndef __DMLFW__LOGISTIC__REGRESSION__
#define __DMLFW__LOGISTIC__REGRESSION__
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_set.h>
#include<inttypes.h>

dmlfw_row_vec_double * dmlfw_logistic_regression_gradient_descent_fit(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost));

dmlfw_column_vec_double * dmlfw_logistic_regression_predict(dmlfw_mat_double *input_features_matrix,dmlfw_row_vec_double *trained_parameters);

dmlfw_mat_double * dmlfw_logistic_regression_gradient_descent_multi_class_fit(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_values_vector,dmlfw_set_string *class_set,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost));


dmlfw_column_vec_double * dmlfw_logistic_regression_multi_class_predict(dmlfw_mat_double *input_features_matrix,dmlfw_set_string *class_set,dmlfw_mat_double *trained_parameters_matrix);
#endif
