#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>

#include<___mlfw_error.h>
extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

enum MLFW_GRADIENT_DESCENT_TYPE{MLFW_BATCH_GRADIENT_DESCENT,MLFW_STOCHASTIC_GRADIENT_DESCENT,MLFW_MINI_BATCH_GRADIENT_DESCENT};

typedef struct _mlfw_gradient_descent_options
{
	double learning_rate;
	uint64_t number_of_iterations;
	int (*progress_callback) (uint64_t iteration_number,void *y,void *predicted_y);
	int gradient_descent_type;
	void (*data_provider)(void *x,void *y,uint64_t starting_row_number,uint32_t number_of_rows_to_provide);
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
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
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
void mlfw_gradient_descent_options_set_progress_callback(mlfw_gradient_descent_options *gd_options,int (*progress_callback)(uint64_t,void *,void *))
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->progress_callback=progress_callback;
}
void mlfw_gradient_descent_options_set_data_provider(mlfw_gradient_descent_options *gd_options,void (*data_provider)(void *,void *,uint64_t,uint32_t))
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
void load_dataset(mlfw_mat_double **x,mlfw_column_vec_double **y)
{
}
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y)
{
	return 0;
}
mlfw_gradient_descent_options * get_gradient_descent_options()
{
	mlfw_gradient_descent_options *gd_options;
	double learning_rate;
	uint64_t number_of_iterations;
	gd_options=mlfw_gradient_descent_options_create_new();
	if(mlfw_error()) return NULL;
	learning_rate=0.001;
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
	mlfw_gradient_descent_options_destroy(gd_options);
	mlfw_get_error_string(error_string,512);
	mlfw_get_debug_string(debug_string,512);
	printf("Model saved to examples-1-model.csv\n");
	return 0;
	err:
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_gradient_descent_options_destroy(gd_options);
	mlfw_get_error_string(error_string,512);
	mlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	return 0;
}
