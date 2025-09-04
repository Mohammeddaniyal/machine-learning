#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>

#include<___mlfw_error.h>
extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

enum MLFW_GRADIENT_DESCENT_TYPE{MLFW_BATCH_GRADIENT_DESCENT,MLFW_STOCHASTIC_GRADIENT_DESCENT,MLFW_MINI_BATCH_GRADIENT_DESCENT};

// parameters: iteration_number,y,predicted_y,model,regularization_parameter
typedef int (*mlfw_gradient_descent_lin_reg_progress_callback_t)(uint64_t,void *,void *,void *,double);

// parameters: x,y,from_row_index,how_many_rows
typedef void (*mlfw_gradient_descent_lin_reg_data_provider_t)(void *,void *,uint64_t,uint64_t);
typedef struct _mlfw_gradient_descent_options
{
	double learning_rate;
	uint64_t number_of_iterations;
	mlfw_gradient_descent_lin_reg_progress_callback_t progress_callback;
	int gradient_descent_type;
	mlfw_gradient_descent_lin_reg_data_provider_t data_provider;
	uint32_t mini_batch_size;
}mlfw_gradient_descent_options;

mlfw_gradient_descent_options * mlfw_gradient_descent_options_create_new()
{
	mlfw_gradient_descent_options *gd_options;
	mlfw_reset_error();
	gd_options=(mlfw_gradient_descent_options *)malloc(sizeof(mlfw_gradient_descent_options));
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(mlfw_gradient_descent_options));
		return NULL;
	}
	gd_options->learning_rate=0.0;
	gd_options->number_of_iterations=0;
	gd_options->progress_callback=NULL;
	gd_options->gradient_descent_type=-1;
	gd_options->data_provider=NULL;
	gd_options->mini_batch_size=0;
	return gd_options;
}
void mlfw_gradient_descent_options_destroy(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	free(gd_options);
}
void mlfw_gradient_descent_options_set_learning_rate(mlfw_gradient_descent_options *gd_options,double learning_rate)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->learning_rate=learning_rate;
}
void mlfw_gradient_descent_options_set_number_of_iterations(mlfw_gradient_descent_options *gd_options,uint64_t number_of_iterations)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		return;
	}
	gd_options->number_of_iterations=number_of_iterations;
}
void mlfw_gradient_descent_options_set_gradient_descent_type(mlfw_gradient_descent_options *gd_options,int gradient_descent_type)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	if(gradient_descent_type==MLFW_BATCH_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_BATCH_GRADIENT_DESCENT;
	}else if(gradient_descent_type==MLFW_STOCHASTIC_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_STOCHASTIC_GRADIENT_DESCENT;
	}else if(gradient_descent_type==MLFW_MINI_BATCH_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_MINI_BATCH_GRADIENT_DESCENT;
	}else
	{
	_mlfw_set_error(MLFW_INVALID_GRADIENT_DESCENT_TYPE_CODE,MLFW_INVALID_GRADIENT_DESCENT_TYPE);
	}
}
void mlfw_gradient_descent_options_set_progress_callback(mlfw_gradient_descent_options *gd_options,mlfw_gradient_descent_progress_callback_t progress_callback)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->progress_callback=progress_callback;
}
void mlfw_gradient_descent_options_set_data_provider(mlfw_gradient_descent_options *gd_options,mlfw_gradient_descent_lin_reg_data_provider_t data_provider)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->data_provider=data_provider;
}
void mlfw_gradient_descent_options_set_mini_batch_size(mlfw_gradient_descent_options *gd_options,uint32_t mini_batch_size)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->mini_batch_size=mini_batch_size;
}
double mlfw_gradient_descent_options_get_learning_rate(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0.0;
	}
	return gd_options->learning_rate;
}
uint64_t mlfw_gradient_descent_options_get_number_of_iterations(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->number_of_iterations;
}
int mlfw_gradient_descent_options_get_gradient_descent_type(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->gradient_descent_type;
}
mlfw_gradient_descent_lin_reg_progress_call_t mlfw_gradient_descent_options_get_progress_callback(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->progress_callback;
}
mlfw_gradient_descent_lin_reg_data_provider_t mlfw_gradient_descent_options_get_data_provider(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->data_provider;
}
uint32_t mlfw_gradient_descent_options_get_mini_batch_size(mlfw_gradient_descent_options *gradient_descent)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->mini_batch_size;
}
mlfw_column_vec_double * mlfw_linear_regression_fit_using_batch_gradient_descent(mlfw_gradient_descent_options  *gd_options,mlfw_mat_double *x,mlfw_column_vec_double *y,double regularization_parameter,mlfw_column_vec_double *model)
{
	dimension_t x_rows,x_columns;
	dimension_t theta_rows;
	double theta_0; // to store value of theta[0] before setting to 0
			// as it is to be ignore in regularization term
	mlfw_gradient_descent_lin_reg_progress_callback_t progress_callback;
	
	mlfw_mat_double *x_transposed;
	mlfw_column_vec_double *predicted_y=NULL;
	mlfw_column_vec_double *prediction_error=NULL;
	mlfw_column_vec_double *theta=NULL;
	mlfw_column_vec_double *tmp1=NULL;
	mlfw_column_vec_double *tmp2=NULL;
	mlfw_column_vec_double *tmp3=NULL;
	mlfw_column_vec_double *tmp4=NULL;
	mlfw_column_vec_double *regularization_term;
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
			_mlfw_set_error(MLFW_INVALID_MODEL_VECTOR_SIZE_CODE,MLFW_INVALID_MODEL_VECTOR_SIZE,"model",size,x_columns);
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

	number_of_iterations=mlfw_gradient_descent_options_get_number_of_iterations(gd_options);
	learning_rate=mlfw_gradient_descent_options_get_learning_rate(gd_options);
	progress_call=mlfw_gradient_descent_options_get_progress_callback(gd_options);
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
		if(mlfw_error()_ goto err;
		// tmp theta calculation, store in tmp_theta=theta-tmp4
		mlfw_subtract_double_column_vector(theta,tmp4,tmp_theta);
		if(mlfw_error()) goto err;
		// copy tmp_theta to theta, to update theta
		mlfw_column_vec_double_copy(theta,tmp_theta);
		if(mlfw_error()) goto err;
		// theta updated
		// iteration complete, call progress_callback, if it returns, -1 break
		if(progress_callback(n,y,predicted_y,theta,regularization_parameter) == -1) break;
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

}
// Everything above this point is being written by Framework Designer
// All the function below this point are being written by Framework User

#define TRAINING_DATASET "IceCreamSales_training_examples.csv"
#define NUMBER_OF_ITERATIONS 3000000
#define LEARNING_RATE 0.0001
#define REGULARIZATION_PARAMETER 0.5
#define FREQUENCY_OF_PRINTING_COST 50000
#define SHOW_GRAPH 1
FILE *gnuplot;

void load_dataset(mlfw_mat_double **x,mlfw_column_vec_double **y)
{
	mlfw_row_vec_string *header;
	dimension_t rows,columns;
	*x=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&header);
	if(mlfw_error()) return;
	mlfw_mat_double_get_dimensions(*x,&rows,&columns);
	if(mlfw_error()) return;
	mlfw_row_vec_string_destroy(header);
	*y=mlfw_mat_double_create_column_vec(*x,columns-1,NULL);
	mlfw_mat_double_reshape(x,rows,columns-1);
}
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y)
{
	printf("Iteration : %" PRIu64 "\n",iteration_number);
	return 1; // keep running
}
mlfw_gradient_descent_options * get_gradient_descent_options()
{
	mlfw_gradient_descent_options *gd_options;
	double learning_rate;
	uint64_t number_of_iterations;
	gd_options=mlfw_gradient_descent_options_create_new();
	if(mlfw_error()) return NULL;
	learning_rate=0.0001;
	number_of_iterations=500000;
	mlfw_gradient_descent_options_set_learning_rate(gd_options,learning_rate);
	mlfw_gradient_descent_options_set_number_of_iterations(gd_options,number_of_iterations);
	mlfw_gradient_descent_options_set_gradient_descent_type(gd_options,MLFW_BATCH_GRADIENT_DESCENT);
	mlfw_gradient_descent_options_set_progress_callback(gd_options,on_iteration_complete);
	return gd_options;
}
int main()
{
	double regularization_parameter;
	char error_string[512];
	char debug_string[512];
	mlfw_column_vec_double *model=NULL;
	mlfw_row_vec_string *model_header=NULL;
	mlfw_mat_double *x=NULL;
	mlfw_column_vec_double *y=NULL;
	mlfw_gradient_descent_options *gd_options=NULL;

	load_dataset(&x,&y);
	regularization_parameter=0.1234;
	if(mlfw_error()) goto err;
	gd_options=get_gradient_descent_options();
	if(mlfw_error()) goto err;
	model=mlfw_linear_regression_fit_using_batch_gradient_descent(gd_options,x,y,regularization_parameter,NULL);
	if(mlfw_error()) goto err;
	model_header=mlfw_row_vec_string_create_new(1);
	if(mlfw_error()) goto err;
	mlfw_row_vec_string_set(model_header,0,"theta");
	if(mlfw_error()) goto err;
	mlfw_column_vec_double_to_csv(model,"example-1-model.csv",model_header);
	if(mlfw_error()) goto err;
	
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	printf("Model saved to examples-1-model.csv\n");
	return 0;
	err:
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	mlfw_get_error_string(error_string,512);
	mlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	return 0;
}
