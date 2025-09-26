#include<stdio.h>
#include<stdlib.h>
#include<___dmlfw_error.h>
#include<dmlfw_error.h>
#include<dmlfw_algorithms.h>
extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];


typedef struct _dmlfw_gradient_descent_options
{
	double learning_rate;
	uint64_t number_of_iterations;
	dmlfw_gradient_descent_lin_reg_progress_callback_t progress_callback;
	int gradient_descent_type;
	dmlfw_gradient_descent_lin_reg_data_provider_t data_provider;
	uint32_t mini_batch_size;
}dmlfw_gradient_descent_options;

dmlfw_gradient_descent_options * dmlfw_gradient_descent_options_create_new()
{
	dmlfw_gradient_descent_options *gd_options;
	dmlfw_reset_error();
	gd_options=(dmlfw_gradient_descent_options *)malloc(sizeof(dmlfw_gradient_descent_options));
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_gradient_descent_options));
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
void dmlfw_gradient_descent_options_destroy(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	free(gd_options);
}
void dmlfw_gradient_descent_options_set_learning_rate(dmlfw_gradient_descent_options *gd_options,double learning_rate)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->learning_rate=learning_rate;
}
void dmlfw_gradient_descent_options_set_number_of_iterations(dmlfw_gradient_descent_options *gd_options,uint64_t number_of_iterations)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		return;
	}
	gd_options->number_of_iterations=number_of_iterations;
}
void dmlfw_gradient_descent_options_set_gradient_descent_type(dmlfw_gradient_descent_options *gd_options,int gradient_descent_type)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
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
	_dmlfw_set_error(MLFW_INVALID_GRADIENT_DESCENT_TYPE_CODE,MLFW_INVALID_GRADIENT_DESCENT_TYPE);
	}
}
void dmlfw_gradient_descent_options_set_progress_callback(dmlfw_gradient_descent_options *gd_options,dmlfw_gradient_descent_lin_reg_progress_callback_t progress_callback)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->progress_callback=progress_callback;
}
void dmlfw_gradient_descent_options_set_data_provider(dmlfw_gradient_descent_options *gd_options,dmlfw_gradient_descent_lin_reg_data_provider_t data_provider)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->data_provider=data_provider;
}
void dmlfw_gradient_descent_options_set_mini_batch_size(dmlfw_gradient_descent_options *gd_options,uint32_t mini_batch_size)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{	
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->mini_batch_size=mini_batch_size;
}
double dmlfw_gradient_descent_options_get_learning_rate(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0.0;
	}
	return gd_options->learning_rate;
}
uint64_t dmlfw_gradient_descent_options_get_number_of_iterations(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->number_of_iterations;
}
int dmlfw_gradient_descent_options_get_gradient_descent_type(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->gradient_descent_type;
}
dmlfw_gradient_descent_lin_reg_progress_callback_t dmlfw_gradient_descent_options_get_progress_callback(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->progress_callback;
}
dmlfw_gradient_descent_lin_reg_data_provider_t dmlfw_gradient_descent_options_get_data_provider(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->data_provider;
}
uint32_t dmlfw_gradient_descent_options_get_mini_batch_size(dmlfw_gradient_descent_options *gd_options)
{
	dmlfw_reset_error();
	if(gd_options==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->mini_batch_size;
}

