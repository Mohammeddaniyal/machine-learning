#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>
#include<unistd.h>

#define TRAINING_DATASET "scaled_poly_trends_train.csv"
#define MODEL_FILE_NAME "example-4-model.csv"
#define ACTUAL_TRAINING_DATASET "trends_train.csv"
#define NUMBER_OF_ITERATIONS 300000000
#define LEARNING_RATE 0.3
#define REGULARIZATION_PARAMETER 0.5
#define FREQUENCY_OF_PRINTING_COST 300000
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
	static mlfw_mat_double *actual_dataset_matrix=NULL;
	static mlfw_row_vec_string *actual_dataset_matrix_header=NULL;
	static dimension_t actual_dataset_matrix_columns=0;
	static dimension_t actual_dataset_matrix_rows=0;

	FILE *cost_file;
	double cost;
	double model_0; // to store value of model[0] before setting to 0
			// as it is to be ignore in regularization term
	double regularization_term;
	double model_squared_sum;

	double sum_of_squared_errors;
	dimension_t y_size;
	dimension_t model_size;
	
	double value;
	double r;

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
	if(actual_dataset_matrix==NULL)
	{
		actual_dataset_matrix=mlfw_mat_double_from_csv(ACTUAL_TRAINING_DATASET,NULL,&actual_dataset_matrix_header);
		if(mlfw_error())
		{
			printf("Unable to load actual training dataset\n");
			return -1;
		}
		mlfw_mat_double_get_dimensions(actual_dataset_matrix,&actual_dataset_matrix_rows,&actual_dataset_matrix_columns);
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
	if(iteration_number%FREQUENCY_OF_PRINTING_COST==0 || iteration_number==NUMBER_OF_ITERATIONS || iteration_number==1)
	{
		printf("Iteration number %lu, Cost %41.15lf\n",iteration_number,cost);
		if(SHOW_GRAPH)
		{
			if(gnuplot!=NULL)
			{
				//fprintf(gnuplot,"set term x11 0 position 20,100 title 'Cost Descent'\n");
				//fflush(gnuplot);
				//fprintf(gnuplot,"set yrange[1:25]\n");
				//fflush(gnuplot);
				//fprintf(gnuplot,"set xrange[1:%u]\n",NUMBER_OF_ITERATIONS);
				//fflush(gnuplot);
				//fprintf(gnuplot,"plot 'cost.csv' notitle\n");
				//fflush(gnuplot);
				// y=mx+c
				
				for(r=0;r<y_size;++r)
				{
					value=mlfw_column_vec_double_get(predicted_y,r);
					mlfw_mat_double_set(actual_dataset_matrix,r,actual_dataset_matrix_columns-1,value);
				}
				mlfw_mat_double_to_csv(actual_dataset_matrix,"curve_fit.csv",actual_dataset_matrix_header);
				fprintf(gnuplot,"set term x11 1 position 700,100 title 'Fitting'\n");
				fflush(gnuplot);
				// How [40:110] [10:110] decided ?
				// plot the data file using externally and analyze the generated
				// x and y range
				//fprintf(gnuplot,"set xrange[40:110]\n");
				//fflush(gnuplot);
				//fprintf(gnuplot,"set yrange[10:110]\n");
				//fflush(gnuplot);
				fprintf(gnuplot,"plot '%s' skip 1 notitle, 'curve_fit.csv' skip 1 notitle\n",ACTUAL_TRAINING_DATASET);
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
		mlfw_mat_double_destroy(actual_dataset_matrix);
		mlfw_row_vec_string_destroy(actual_dataset_matrix_header);
	}
	return 0;

	err:
		mlfw_column_vec_double_destroy(prediction_error);
		mlfw_row_vec_double_destroy(prediction_error_transposed);
		mlfw_column_vec_double_destroy(product_vector);
		mlfw_row_vec_double_destroy(model_transposed);
		mlfw_column_vec_double_destroy(model_squared_sum_vector);
		mlfw_mat_double_destroy(actual_dataset_matrix);
		mlfw_row_vec_string_destroy(actual_dataset_matrix_header);
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
		printf("Waiting for resources to be cleared, press ctrl+c if it takes too long\n");
		sleep(10);
		pclose(gnuplot);
	}
	return 0;
}
