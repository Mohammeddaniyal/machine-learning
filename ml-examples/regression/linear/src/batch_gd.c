/**
 * @file batch_gd.c
 * @brief Demonstrates batch gradient descent for linear regression using ml-framework.
 * @ingroup ml-examples-regression-linear
 * @{
 *
 * @author Mohammed Daniyal
 * @version 1.0
 * @date 2025-09-26
 *
 * This example loads the IceCreamSales training dataset, trains a linear regression model
 * using batch gradient descent with regularization, tracks cost progress,
 * optionally plots cost and fitted line using gnuplot, and saves the final model to CSV.
 *
 * Defines custom progress callback for iteration logging and visualization.
 *
 * Usage:
 *   ./batch_gd
 *
 */
#include<stdio.h>
#include<dmlfw.h>
#include<stdlib.h>
#include<unistd.h>

/** Training dataset CSV file path */
#define TRAINING_DATASET "IceCreamSales_training_examples.csv"

/** Output model CSV file path */
#define MODEL_FILE_NAME "example-1-model.csv"

/** Maximum number of iterations for gradient descent */
#define NUMBER_OF_ITERATIONS 3000000

/** Learning rate for gradient descent */
#define LEARNING_RATE 0.0001

/** Regularization parameter (lambda) */
#define REGULARIZATION_PARAMETER 0.5

/** Frequency of printing/logging cost during training */
#define FREQUENCY_OF_PRINTING_COST 50000

/** Flag to enable (1) or disable (0) graph plotting */
#define SHOW_GRAPH 1

/** Gnuplot file pointer used for plotting progress */
FILE *gnuplot;

/**
 * @brief Prints error string from ml-framework and exits program.
 */
void print_error_and_exit()
{
	char error_string[512];
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	exit(0);
}

/**
 * @brief Loads the dataset into X (features matrix) and Y (target vector).
 *        Adds bias column filled with 1.0.
 *
 * @param[out] x Pointer to the features matrix pointer.
 * @param[out] y Pointer to the target column vector pointer.
 */
void load_dataset(dmlfw_mat_double **x,dmlfw_column_vec_double **y)
{
	dmlfw_mat_double *matrix=NULL;
	dimension_t matrix_rows,matrix_columns;
	dmlfw_row_vec_string *header=NULL;
	matrix=dmlfw_mat_double_from_csv(TRAINING_DATASET,NULL,&header);
	if(dmlfw_error()) 
	{
		print_error_and_exit();
	}
	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	*y=dmlfw_mat_double_create_column_vec(matrix,matrix_columns-1,NULL);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		print_error_and_exit();
	}
	dmlfw_mat_double_truncate(&matrix,0,0,matrix_rows-1,matrix_columns-2);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// create column for bias
	dmlfw_mat_double_insert_columns(&matrix,0,1);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// fill with 1.0
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	dmlfw_mat_double_fill(matrix,0,0,matrix_rows-1,0,1.0);
	*x=matrix;
}
/**
 * @brief Progress callback called on each gradient descent iteration.
 *        Logs cost, updates plot files, shows graph via gnuplot at intervals.
 *
 * @param iteration_number Current iteration number.
 * @param y Actual target vector.
 * @param predicted_y Predicted target vector.
 * @param model Current model parameters.
 * @param regularization_parameter Regularization coefficient lambda.
 * @return 0 on success, negative to abort.
 */
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y,void *model,double regularization_parameter)
{
	static dmlfw_column_vec_double *prediction_error=NULL;
	static dmlfw_row_vec_double *prediction_error_transposed=NULL;
	static dmlfw_column_vec_double *product_vector=NULL;
	static dmlfw_row_vec_double *model_transposed=NULL;
	static dmlfw_column_vec_double *model_squared_sum_vector=NULL;

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
	y_size=dmlfw_column_vec_double_get_size(y);
	model_size=dmlfw_column_vec_double_get_size(model);

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
		prediction_error=dmlfw_column_vec_double_create_new(y_size);
		if(dmlfw_error()) goto err;
		prediction_error_transposed=dmlfw_row_vec_double_create_new(y_size);
		if(dmlfw_error()) goto err;
		product_vector=dmlfw_column_vec_double_create_new(1);
		if(dmlfw_error()) goto err;
		model_transposed=dmlfw_row_vec_double_create_new(model_size);
		if(dmlfw_error()) goto err;
		model_squared_sum_vector=dmlfw_column_vec_double_create_new(1);
		if(dmlfw_error()) goto err;
	}
	// calculate error
	dmlfw_subtract_double_column_vector(predicted_y,y,prediction_error);
	if(dmlfw_error()) goto err;
	// calculate error squared sum
	dmlfw_column_vec_double_transpose(prediction_error,prediction_error_transposed);
	if(dmlfw_error()) goto err;
	dmlfw_multiply_double_row_vector_with_column_vector(prediction_error_transposed,prediction_error,product_vector);
	if(dmlfw_error()) goto err;
	sum_of_squared_errors=dmlfw_column_vec_double_get(product_vector,0);

	// pick model[0] and set model[0] to 0 as we want to ignore
	// bias while calculating regularization_term
	model_0=dmlfw_column_vec_double_get(model,0);
	dmlfw_column_vec_double_set(model,0,0.0);
	dmlfw_column_vec_double_transpose(model,model_transposed);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_set(model,0,model_0);
		goto err;
	}
	// calculate model/theta squared sum
	dmlfw_multiply_double_row_vector_with_column_vector(model_transposed,model,model_squared_sum_vector);
	dmlfw_column_vec_double_set(model,0,model_0);
	if(dmlfw_error()) goto err;
	model_squared_sum=dmlfw_column_vec_double_get(model_squared_sum_vector,0);
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
				c=dmlfw_column_vec_double_get(model,0);
				m=dmlfw_column_vec_double_get(model,1);
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
		dmlfw_column_vec_double_destroy(prediction_error);
		dmlfw_row_vec_double_destroy(prediction_error_transposed);
		dmlfw_column_vec_double_destroy(product_vector);
		dmlfw_row_vec_double_destroy(model_transposed);
		dmlfw_column_vec_double_destroy(model_squared_sum_vector);
	}
	return 0;

	err:
		dmlfw_column_vec_double_destroy(prediction_error);
		dmlfw_row_vec_double_destroy(prediction_error_transposed);
		dmlfw_column_vec_double_destroy(product_vector);
		dmlfw_row_vec_double_destroy(model_transposed);
		dmlfw_column_vec_double_destroy(model_squared_sum_vector);
		return -1;
}
/**
 * @brief Creates and configures gradient descent options object.
 *
 * Sets learning rate, number of iterations, gradient descent type,
 * and associates the progress callback.
 *
 * @return Pointer to configured dmlfw_gradient_descent_options or NULL on error.
 */
dmlfw_gradient_descent_options * get_gradient_descent_options()
{
	dmlfw_gradient_descent_options *gd_options;
	double learning_rate;
	uint64_t number_of_iterations;
	gd_options=dmlfw_gradient_descent_options_create_new();
	if(dmlfw_error()) return NULL;
	learning_rate=LEARNING_RATE;
	number_of_iterations=NUMBER_OF_ITERATIONS;
	dmlfw_gradient_descent_options_set_learning_rate(gd_options,learning_rate);
	dmlfw_gradient_descent_options_set_number_of_iterations(gd_options,number_of_iterations);
	dmlfw_gradient_descent_options_set_gradient_descent_type(gd_options,MLFW_BATCH_GRADIENT_DESCENT);
	dmlfw_gradient_descent_options_set_progress_callback(gd_options,on_iteration_complete);
	return gd_options;
}
/**
 * @brief Main function to execute batch gradient descent linear regression example.
 *
 * Loads dataset, creates gradient descent options, fits model,
 * saves model parameters to CSV, and manages resources.
 *
 * @return 0 on success.
 */
int main()
{
	double regularization_parameter;
	char error_string[512];
	char debug_string[512];
	dmlfw_column_vec_double *model=NULL;// for parameters
	dmlfw_row_vec_string *model_header=NULL;
	dmlfw_mat_double *x=NULL;
	dmlfw_column_vec_double *y=NULL;
	dmlfw_gradient_descent_options *gd_options=NULL;

	load_dataset(&x,&y); // function defined by framework user
	regularization_parameter=REGULARIZATION_PARAMETER;
	if(dmlfw_error()) goto err;
	gd_options=get_gradient_descent_options(); // function defined by framework user
	if(dmlfw_error()) goto err;
	model=dmlfw_linear_regression_fit_using_batch_gradient_descent(gd_options,x,y,regularization_parameter,NULL);
	if(dmlfw_error()) goto err;
	model_header=dmlfw_row_vec_string_create_new(1);
	if(dmlfw_error()) goto err;
	dmlfw_row_vec_string_set(model_header,0,"theta");
	if(dmlfw_error()) goto err;
	dmlfw_column_vec_double_to_csv(model,MODEL_FILE_NAME,model_header);
	if(dmlfw_error()) goto err;
	
	printf("Model saved to %s\n",MODEL_FILE_NAME);
	dmlfw_mat_double_destroy(x);
	dmlfw_column_vec_double_destroy(y);
	dmlfw_column_vec_double_destroy(model);
	dmlfw_row_vec_string_destroy(model_header);
	dmlfw_gradient_descent_options_destroy(gd_options);
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
	dmlfw_get_error_string(error_string,512);
	dmlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	dmlfw_mat_double_destroy(x);
	dmlfw_column_vec_double_destroy(y);
	dmlfw_column_vec_double_destroy(model);
	dmlfw_row_vec_string_destroy(model_header);
	dmlfw_gradient_descent_options_destroy(gd_options);
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
/** @} */
