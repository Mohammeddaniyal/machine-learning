#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>

#include<___mlfw_error.h>
extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

enum MLFW_GRADIENT_DESCENT_TYPE{MLFW_BATCH_GRADIENT_DESCENT,MLFW_STOCHASTIC_GRADIENT_DESCENT,MLFW_MINI_BATCH_GRADIENT_DESCENT};

typedef struc _mlfw_gradient_descent_options
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
