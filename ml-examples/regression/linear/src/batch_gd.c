#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>
#include<unistd.h>
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
void mlfw_gradient_descent_options_set_progress_callback(mlfw_gradient_descent_options *gd_options,mlfw_gradient_descent_lin_reg_progress_callback_t progress_callback)
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
double mlfw_gradient_descent_options_get_learning_rate(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0.0;
	}
	return gd_options->learning_rate;
}
uint64_t mlfw_gradient_descent_options_get_number_of_iterations(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->number_of_iterations;
}
int mlfw_gradient_descent_options_get_gradient_descent_type(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->gradient_descent_type;
}
mlfw_gradient_descent_lin_reg_progress_callback_t mlfw_gradient_descent_options_get_progress_callback(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->progress_callback;
}
mlfw_gradient_descent_lin_reg_data_provider_t mlfw_gradient_descent_options_get_data_provider(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return 0;
	}
	return gd_options->data_provider;
}
uint32_t mlfw_gradient_descent_options_get_mini_batch_size(mlfw_gradient_descent_options *gd_options)
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
// Everything above this point is being written by Framework Designer
// All the function below this point are being written by Framework User

#define TRAINING_DATASET "IceCreamSales_training_examples.csv"
#define MODEL_FILE_NAME "example-1-model.csv"
#define NUMBER_OF_ITERATIONS 3000000
#define LEARNING_RATE 0.0001
#define REGULARIZATION_PARAMETER 0.5
#define FREQUENCY_OF_PRINTING_COST 50000
#define SHOW_GRAPH 1
FILE *gnuplot;

void print_error_and_exit()
{
	char error_string[512];
	mlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	exit(0);
}

void load_dataset(mlfw_mat_double **x,mlfw_column_vec_double **y)
{
	mlfw_mat_double *matrix=NULL;
	dimension_t matrix_rows,matrix_columns;
	mlfw_row_vec_string *header=NULL;
	matrix=mlfw_mat_double_from_csv(TRAINING_DATASET,NULL,&header);
	if(mlfw_error()) 
	{
		print_error_and_exit();
	}
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	*y=mlfw_mat_double_create_column_vec(matrix,matrix_columns-1,NULL);
	if(mlfw_error()) 
	{
		mlfw_mat_double_destroy(matrix);
		print_error_and_exit();
	}
	mlfw_mat_double_truncate(&matrix,0,0,matrix_rows-1,matrix_columns-2);
	if(mlfw_error()) 
	{
		mlfw_mat_double_destroy(matrix);
		mlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// create column for bias
	mlfw_mat_double_insert_columns(&matrix,0,1);
	if(mlfw_error()) 
	{
		mlfw_mat_double_destroy(matrix);
		mlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// fill with 1.0
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	mlfw_mat_double_fill(matrix,0,0,matrix_rows-1,0,1.0);
	*x=matrix;
}
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y,void *model,double regularization_parameter)
{
	static mlfw_column_vec_double *prediction_error=NULL;
	static mlfw_row_vec_double *prediction_error_transposed=NULL;
	static mlfw_column_vec_double *product_vector=NULL;
	static mlfw_row_vec_double *model_transposed=NULL;
	static mlfw_column_vec_double *model_squared_sum_vector=NULL;

	FILE *cost_file;
	FILE *line_fit_file;
	double cost;
	double model_0; // to store value of model[0] before setting to 0
			// as it is to be ignore in regularization term
	double regularization_term;
	double model_squared_sum;

	double sum_of_squared_errors;
	dimension_t y_size;
	dimension_t model_size;
	// for line fitting graph using (y=mx+c)	
	double x1,x2,y1,y2;
	double m,c;

	if(y==NULL)
	{
		printf("y vector is NULL\n");
		return -1;
	}
	if(predicted_y==NULL)
	{
		printf("predicted_y vector is NULL\n");
		return -1;
	}
	if(model==NULL)
	{
		printf("model vector is NULL\n");
		return -1;
	}
	y_size=mlfw_column_vec_double_get_size(y);
	model_size=mlfw_column_vec_double_get_size(model);

	if(iteration_number==1)
	{
		// one time act, at the start
		if(SHOW_GRAPH)
		{
			gnuplot=popen("gnuplot","w");
			fprintf(gnuplot,"set datafile separator ','\n");
			fflush(gnuplot);
			/* [1:25] How ?
			 * After analyzing the cost initially without displaying
			 * the cost graph, this 1:25 was decided, the set the
			 * SHOW_COST_GRAPH to 1 and frequency to 100000
			 */
			cost_file=fopen("cost.csv","w");
			fclose(cost_file); // ensure cost.csv is empty
		}
		prediction_error=mlfw_column_vec_double_create_new(y_size);
		if(mlfw_error()) goto err;
		prediction_error_transposed=mlfw_row_vec_double_create_new(y_size);
		if(mlfw_error()) goto err;
		product_vector=mlfw_column_vec_double_create_new(1);
		if(mlfw_error()) goto err;
		model_transposed=mlfw_row_vec_double_create_new(model_size);
		if(mlfw_error()) goto err;
		model_squared_sum_vector=mlfw_column_vec_double_create_new(1);
		if(mlfw_error()) goto err;
	}
	// calculate error
	mlfw_subtract_double_column_vector(predicted_y,y,prediction_error);
	if(mlfw_error()) goto err;
	// calculate error squared sum
	mlfw_column_vec_double_transpose(prediction_error,prediction_error_transposed);
	if(mlfw_error()) goto err;
	mlfw_multiply_double_row_vector_with_column_vector(prediction_error_transposed,prediction_error,product_vector);
	if(mlfw_error()) goto err;
	sum_of_squared_errors=mlfw_column_vec_double_get(product_vector,0);

	// pick model[0] and set model[0] to 0 as we want to ignore
	// bias while calculating regularization_term
	model_0=mlfw_column_vec_double_get(model,0);
	mlfw_column_vec_double_set(model,0,0.0);
	mlfw_column_vec_double_transpose(model,model_transposed);
	if(mlfw_error())
	{
		mlfw_column_vec_double_set(model,0,model_0);
		goto err;
	}
	// calculate model/theta squared sum
	mlfw_multiply_double_row_vector_with_column_vector(model_transposed,model,model_squared_sum_vector);
	mlfw_column_vec_double_set(model,0,model_0);
	if(mlfw_error()) goto err;
	model_squared_sum=mlfw_column_vec_double_get(model_squared_sum_vector,0);
	// calculate regularization term
	regularization_term=(regularization_parameter/(2*y_size))*model_squared_sum;

	// calculate cost
	cost=(sum_of_squared_errors/(2*y_size))+regularization_term;
	if(SHOW_GRAPH)
	{
		if(gnuplot!=NULL)
		{
			cost_file=fopen("cost.csv","a");
			fprintf(cost_file,"%lu,%f\n",iteration_number,cost);
			fclose(cost_file);
		}
	}
	if(iteration_number%FREQUENCY_OF_PRINTING_COST==0 || iteration_number==NUMBER_OF_ITERATIONS)
	{
		printf("Iteration number %lu, Cost %41.15lf\n",iteration_number,cost);
		if(SHOW_GRAPH)
		{
			if(gnuplot!=NULL)
			{
				fprintf(gnuplot,"set term x11 0 position 20,100 title 'Cost Descent'\n");
				fflush(gnuplot);
				fprintf(gnuplot,"set yrange[1:25]\n");
				fflush(gnuplot);
				fprintf(gnuplot,"set xrange[1:%u]\n",NUMBER_OF_ITERATIONS);
				fflush(gnuplot);
				fprintf(gnuplot,"plot 'cost.csv' notitle\n");
				fflush(gnuplot);
				// y=mx+c
				c=mlfw_column_vec_double_get(model,0);
				m=mlfw_column_vec_double_get(model,1);
				x1=0.0;
				y1=m*x1+c;
				x2=110.0;
				y2=m*x2+c;
				line_fit_file=fopen("line.csv","w");
				fprintf(line_fit_file,"%lf,%lf\n",x1,y1);
				fprintf(line_fit_file,"%lf,%lf\n",x2,y2);
				fclose(line_fit_file);
				fprintf(gnuplot,"set term x11 1 position 700,100 title 'Line Fitting'\n");
				fflush(gnuplot);
				// How [40:110] [10:110] decided ?
				// plot the data file using externally and analyze the generated
				// x and y range
				fprintf(gnuplot,"set xrange[40:110]\n");
				fflush(gnuplot);
				fprintf(gnuplot,"set yrange[10:110]\n");
				fflush(gnuplot);
				fprintf(gnuplot,"plot '%s' skip 1 notitle, 'line.csv' with lines lw 3 notitle\n",TRAINING_DATASET);
				fflush(gnuplot);
			}
		}
	}
	if(iteration_number==NUMBER_OF_ITERATIONS)
	{
		// one time act, at the end
		mlfw_column_vec_double_destroy(prediction_error);
		mlfw_row_vec_double_destroy(prediction_error_transposed);
		mlfw_column_vec_double_destroy(product_vector);
		mlfw_row_vec_double_destroy(model_transposed);
		mlfw_column_vec_double_destroy(model_squared_sum_vector);
	}
	return 0;

	err:
		mlfw_column_vec_double_destroy(prediction_error);
		mlfw_row_vec_double_destroy(prediction_error_transposed);
		mlfw_column_vec_double_destroy(product_vector);
		mlfw_row_vec_double_destroy(model_transposed);
		mlfw_column_vec_double_destroy(model_squared_sum_vector);
		return -1;
}
mlfw_gradient_descent_options * get_gradient_descent_options()
{
	mlfw_gradient_descent_options *gd_options;
	double learning_rate;
	uint64_t number_of_iterations;
	gd_options=mlfw_gradient_descent_options_create_new();
	if(mlfw_error()) return NULL;
	learning_rate=LEARNING_RATE;
	number_of_iterations=NUMBER_OF_ITERATIONS;
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
	mlfw_column_vec_double *model=NULL;// for parameters
	mlfw_row_vec_string *model_header=NULL;
	mlfw_mat_double *x=NULL;
	mlfw_column_vec_double *y=NULL;
	mlfw_gradient_descent_options *gd_options=NULL;

	load_dataset(&x,&y); // function defined by framework user
	regularization_parameter=REGULARIZATION_PARAMETER;
	if(mlfw_error()) goto err;
	gd_options=get_gradient_descent_options(); // function defined by framework user
	if(mlfw_error()) goto err;
	model=mlfw_linear_regression_fit_using_batch_gradient_descent(gd_options,x,y,regularization_parameter,NULL);
	if(mlfw_error()) goto err;
	model_header=mlfw_row_vec_string_create_new(1);
	if(mlfw_error()) goto err;
	mlfw_row_vec_string_set(model_header,0,"theta");
	if(mlfw_error()) goto err;
	mlfw_column_vec_double_to_csv(model,MODEL_FILE_NAME,model_header);
	if(mlfw_error()) goto err;
	
	printf("Model saved to %s\n",MODEL_FILE_NAME);
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	if(gnuplot!=NULL)
	{
		fprintf(gnuplot,"exit\n");
		fflush(gnuplot);
		printf("Waiting for resources to be cleared, press ctrl+c it it takes too long\n");
		sleep(10); // reason M4 lec 14 21:28 
		pclose(gnuplot);
	}
	return 0;
	err:
	mlfw_get_error_string(error_string,512);
	mlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	if(gnuplot!=NULL)
	{
		fprintf(gnuplot,"exit\n");
		fflush(gnuplot);
		printf("Waiting for resources to be cleared, press ctrl+c it it takes too long\n");
		sleep(10);
		pclose(gnuplot);
	}
	return 0;
}
