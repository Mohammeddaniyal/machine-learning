#include<stdio.h>
#include<mlfw_linear_regression.h>
#include<mlfw_vector.h>
#include<mlfw_matrix.h>
#include<___mlfw_error.h>
#include<mlfw_error.h>
#include<stdlib.h>
extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

mlfw_column_vec_double * mlfw_linear_regression_fit_using_batch_gradient_descent(mlfw_gradient_descent_options  *gd_options,mlfw_mat_double *x,mlfw_column_vec_double *y,double regularization_parameter,mlfw_column_vec_double *model)
{
	dimension_t x_rows,x_columns;
	dimension_t theta_rows;
	double theta_0; // to store value of theta[0] before setting to 0
			// as it is to be ignore in regularization term
	mlfw_gradient_descent_lin_reg_progress_callback_t progress_callback;
	
	mlfw_mat_double *x_transposed=NULL;
	mlfw_column_vec_double *predicted_y=NULL;
	mlfw_column_vec_double *prediction_error=NULL;
	mlfw_column_vec_double *theta=NULL;
	mlfw_column_vec_double *tmp1=NULL;
	mlfw_column_vec_double *tmp2=NULL;
	mlfw_column_vec_double *tmp3=NULL;
	mlfw_column_vec_double *tmp4=NULL;
	mlfw_column_vec_double *regularization_term=NULL;
	mlfw_column_vec_double *tmp_theta=NULL;

	index_t n;
	uint64_t number_of_iterations;
	double learning_rate;
	dimension_t model_size;

	// validations
	x_transposed=mlfw_mat_double_transpose(x,NULL);
	if(mlfw_error()) goto err;
	// setup initial parameters
	mlfw_mat_double_get_dimensions(x,&x_rows,&x_columns);
	theta=mlfw_column_vec_double_create_new_filled(x_columns,0.0,NULL);
	if(mlfw_error()) goto err;

	if(model!=NULL)
	{	
		// validate model size
		model_size=mlfw_column_vec_double_get_size(model);
		if(model_size!=x_columns)
		{
			_mlfw_set_error(MLFW_INVALID_MODEL_VECTOR_SIZE_CODE,MLFW_INVALID_MODEL_VECTOR_SIZE,"model",model_size,x_columns);
			goto err;
		}
		// populate theta with values from model
		mlfw_column_vec_double_copy(theta,model);
		if(mlfw_error()) goto err;
	}
	// created vector for storing predicted values
	theta_rows=x_columns;
	predicted_y=mlfw_column_vec_double_create_new(x_rows);
	if(mlfw_error()) goto err;
	prediction_error=mlfw_column_vec_double_create_new(x_rows);
	if(mlfw_error()) goto err;
	tmp1=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;
	tmp2=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;
	tmp3=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;
	tmp4=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;
	tmp_theta=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;
	regularization_term=mlfw_column_vec_double_create_new(theta_rows);
	if(mlfw_error()) goto err;

	// i can also apply validation here
	number_of_iterations=mlfw_gradient_descent_options_get_number_of_iterations(gd_options);
	learning_rate=mlfw_gradient_descent_options_get_learning_rate(gd_options);
	progress_callback=mlfw_gradient_descent_options_get_progress_callback(gd_options);
	n=1;
	while(n<=number_of_iterations)
	{
		// predict
		mlfw_multiply_double_matrix_with_column_vector(x,theta,predicted_y);
		if(mlfw_error()) goto err;
		// compute error
		mlfw_subtract_double_column_vector(predicted_y,y,prediction_error);
		if(mlfw_error()) goto err;
		// tmp calculation, store in tmp1=x_transposed * prediction_error
		mlfw_multiply_double_matrix_with_column_vector(x_transposed,prediction_error,tmp1);
		if(mlfw_error()) goto err;
		// tmp calculation, store in tmp2=(1.0/x_rows))*tmp2;
		mlfw_multiply_double_scalar_with_column_vector(1.0/x_rows,tmp1,tmp2);
		if(mlfw_error()) goto err;
		// pick theta[0]
		theta_0=mlfw_column_vec_double_get(theta,0);
		if(mlfw_error()) goto err;
		// set theta[0] for regularization
		mlfw_column_vec_double_set(theta,0,0.0);
		if(mlfw_error()) goto err;
		// regularization_term calculation, store in regularization_term=lambda/m*theta (ignoring_bias)
		mlfw_multiply_double_scalar_with_column_vector(regularization_parameter/x_rows,theta,regularization_term);
		if(mlfw_error()) goto err;
		// set back theta[0], what we picked into theta_0
		mlfw_column_vec_double_set(theta,0,theta_0);
		if(mlfw_error()) goto err;
		// tmp calculation, tmp3=tmp2+regularization_term
		mlfw_add_double_column_vector(tmp2,regularization_term,tmp3);
		if(mlfw_error()) goto err;
		// tmp calculation, tmp4=learning_rate*tmp3
		mlfw_multiply_double_scalar_with_column_vector(learning_rate,tmp3,tmp4);
		if(mlfw_error()) goto err;
		// tmp theta calculation, store in tmp_theta=theta-tmp4
		mlfw_subtract_double_column_vector(theta,tmp4,tmp_theta);
		if(mlfw_error()) goto err;
		// copy tmp_theta to theta, to update theta
		mlfw_column_vec_double_copy(theta,tmp_theta);
		if(mlfw_error()) goto err;
		// theta updated
		// iteration complete, call progress_callback, if it returns, -1 break
		if(progress_callback!=NULL)
		{
		if(progress_callback(n,y,predicted_y,theta,regularization_parameter) == -1) break;
		}
		++n;
		}
		mlfw_mat_double_destroy(x_transposed);
		mlfw_column_vec_double_destroy(predicted_y);
		mlfw_column_vec_double_destroy(prediction_error);
		mlfw_column_vec_double_destroy(tmp1);
		mlfw_column_vec_double_destroy(tmp2);
		mlfw_column_vec_double_destroy(tmp3);
		mlfw_column_vec_double_destroy(tmp4);
		mlfw_column_vec_double_destroy(tmp_theta);
		mlfw_column_vec_double_destroy(regularization_term);
		return theta;
		err:
		mlfw_mat_double_destroy(x_transposed);
		mlfw_column_vec_double_destroy(predicted_y);
		mlfw_column_vec_double_destroy(prediction_error);
		mlfw_column_vec_double_destroy(tmp1);
		mlfw_column_vec_double_destroy(tmp2);
		mlfw_column_vec_double_destroy(tmp3);
		mlfw_column_vec_double_destroy(tmp4);
		mlfw_column_vec_double_destroy(tmp_theta);
		mlfw_column_vec_double_destroy(regularization_term);
		return NULL;
	

}
mlfw_column_vec_double * mlfw_linear_regression_predict(mlfw_mat_double *x,mlfw_column_vec_double *model)
{
	mlfw_column_vec_double *predicted_y;
	mlfw_reset_error();
	predicted_y=mlfw_multiply_double_matrix_with_column_vector(x,model,NULL);
	if(mlfw_error()) return NULL;
	return predicted_y;
}

