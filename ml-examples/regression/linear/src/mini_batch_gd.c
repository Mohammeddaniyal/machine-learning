#include<mlfw.h>
#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<___mlfw_error.h>

extern __thread uint64_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

mlfw_column_vec_double * mlfw_linear_regression_fit_using_mini_batch_gradient_descent(mlfw_gradient_descent_options *gd_options,double regularization_parameter,mlfw_column_vec_double *model)
{
	int end_process=0;
	uint64_t from_row_number;
	uint32_t how_many_rows;
	dimension_t x_rows,x_columns;
	dimension_t theta_rows;
	double theta_0; // to store value of theta[0] before setting to 0
				 // as it is to be ignore in regularization term
	mlfw_gradient_descent_lin_reg_progress_callback_t progress_callback;

	mlfw_mat_double *x=NULL;
	mlfw_column_vec_double *y=NULL;	
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
	mlfw_gradient_descent_lin_reg_data_provider_t data_provider;
	uint32_t mini_batch_size;
	mlfw_reset_error();
	// validations
	// i can also apply validation here
	number_of_iterations=mlfw_gradient_descent_options_get_number_of_iterations(gd_options);
	learning_rate=mlfw_gradient_descent_options_get_learning_rate(gd_options);
	progress_callback=mlfw_gradient_descent_options_get_progress_callback(gd_options);
	data_provider=mlfw_gradient_descent_options_get_data_provider(gd_options);
	mini_batch_size=mlfw_gradient_descent_options_get_mini_batch_size(gd_options);
	if(data_provider==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options[data_provider]");
		goto err;
	}

	// get first data, so that we can perform initial setup
	how_many_rows=mini_batch_size;
	data_provider(&x,&y,1,how_many_rows);
	if(x==NULL)
	{
		_mlfw_set_error(MLFW_DATA_PROVIDER_PROVIDED_NO_ROWS_CODE,MLFW_DATA_PROVIDER_PROVIDED_NO_ROWS);
		goto err;
	}
	// set up initial parameters
	mlfw_mat_double_get_dimensions(x,&x_rows,&x_columns);
	theta=mlfw_column_vec_double_create_new_filled(x_columns,0.0,NULL);
	if(mlfw_error()) goto err;
	x_transposed=mlfw_mat_double_transpose(x,NULL);
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
	
	n=1;
	while(1)
	{
	if(n>number_of_iterations) break;
	from_row_number=1; // we are fetching 1, earlier to just for initial setup
	while(1)
	{
		data_provider(&x,&y,from_row_number,how_many_rows);
		if(mlfw_error())
		{
			goto err;
		}
		if(x==NULL)
		{
			break;
		}
			//transpose x
				mlfw_mat_double_transpose(x,x_transposed);
				if(mlfw_error()) goto err;
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
		end_process=progress_callback(n,y,predicted_y,theta,regularization_parameter);
		}
		from_row_number++;
		}// infinite loop ends
		if(end_process==-1) break;
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
		mlfw_column_vec_double_destroy(theta);
		mlfw_column_vec_double_destroy(tmp1);
		mlfw_column_vec_double_destroy(tmp2);
		mlfw_column_vec_double_destroy(tmp3);
		mlfw_column_vec_double_destroy(tmp4);
		mlfw_column_vec_double_destroy(tmp_theta);
		mlfw_column_vec_double_destroy(regularization_term);
		return NULL; 
}



#define TRAINING_DATASET "IceCreamSales_training_examples.csv"
#define MODEL_FILE_NAME "example-3-model.csv"
#define NUMBER_OF_ITERATIONS 100000
#define LEARNING_RATE 0.00001
#define REGULARIZATION_PARAMETER 0.5
#define FREQUENCY_OF_PRINTING_COST 100
#define SHOW_GRAPH 1
uint32_t BUFFER_SIZE=50; // not a macro as it may have to be changed
uint32_t MINI_BATCH_SIZE=46; // not a macro as it may have to be changed



	FILE *gnuplot;
	// following global variables used in get_from_file_buffer & init_buffers
	uint64_t number_of_training_examples=0;
	uint64_t number_of_columns_in_training_examples=0;
	mlfw_mat_double *buffer_matrix_1=NULL;
	mlfw_mat_double *buffer_matrix_2=NULL;
	mlfw_mat_double *buffer_matrix_1_shuffled=NULL;
	mlfw_mat_double *buffer_matrix_2_shuffled=NULL;
	
	// following global variables used in progress_callback (on_iteration_complete)
	mlfw_column_vec_double *prediction_error=NULL;
	mlfw_row_vec_double *prediction_error_transposed=NULL;
	mlfw_column_vec_double *product_vector=NULL;
	mlfw_row_vec_double *model_transposed=NULL;
	mlfw_column_vec_double *model_squared_sum_vector=NULL;

	// following global variables used in data_provider (data_loader)
	mlfw_mat_double *x_matrix=NULL;
	mlfw_column_vec_double *y_vector=NULL;
	mlfw_mat_double *xy_matrix=NULL;


void print_error_and_exit()
{
	char error_string[512];
	mlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	exit(0);
}




void init_buffers()
{
	mlfw_get_csv_dimensions(TRAINING_DATASET,&number_of_training_examples,&number_of_columns_in_training_examples);
	if(mlfw_error()) return;
	if(number_of_training_examples<MINI_BATCH_SIZE)
	{
		MINI_BATCH_SIZE=number_of_training_examples;
	}
	if((number_of_training_examples%MINI_BATCH_SIZE)!=0)
	{
		// compromise with some training examples
		
		number_of_training_examples=number_of_training_examples-(number_of_training_examples%MINI_BATCH_SIZE);
	}
	if(BUFFER_SIZE<MINI_BATCH_SIZE) BUFFER_SIZE=MINI_BATCH_SIZE;
	if((BUFFER_SIZE%MINI_BATCH_SIZE)!=0)
	{
		// make BUFFER_SIZE MULTIPLE of MINI_BATCH_SIZE
		BUFFER_SIZE=BUFFER_SIZE+(MINI_BATCH_SIZE-(BUFFER_SIZE%MINI_BATCH_SIZE));
	}
	if(number_of_training_examples>=BUFFER_SIZE)
	{
		buffer_matrix_1=mlfw_mat_double_create_new(BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
		buffer_matrix_1_shuffled=mlfw_mat_double_create_new(BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	if(number_of_training_examples%BUFFER_SIZE>0)
	{
		buffer_matrix_2=mlfw_mat_double_create_new(number_of_training_examples%BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
		buffer_matrix_2_shuffled=mlfw_mat_double_create_new(number_of_training_examples%BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
	}
	else
	{
		buffer_matrix_2=mlfw_mat_double_create_new(number_of_training_examples,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
		buffer_matrix_2_shuffled=mlfw_mat_double_create_new(number_of_training_examples,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
}

void get_from_file_buffer(mlfw_mat_double **target_matrix,uint64_t from_row,uint32_t how_many_rows)
{
	// this function will only get called till records are available
	// it won't happen that  the demand is invalid
	
	static uint64_t buffer_matrix_1_starts_at_row=0;
	static uint64_t buffer_matrix_2_starts_at_row=0;
	static uint64_t buffer_matrix_1_ends_at_row=0;
	static uint8_t buffers_populated=0;

	static long f_position=0;

	dimension_t buffer_matrix_1_rows=0;
	dimension_t buffer_matrix_1_columns=0;
	dimension_t buffer_matrix_2_rows=0;
	dimension_t buffer_matrix_2_columns=0;

	uint64_t buffer_matrix_1_last_possible_row;

	index_t from_buffer_matrix_index;
	index_t to_buffer_matrix_index;

	if(buffer_matrix_1!=NULL)
	{
		mlfw_mat_double_get_dimensions(buffer_matrix_1,&buffer_matrix_1_rows,&buffer_matrix_1_columns);
	}
	if(buffer_matrix_2!=NULL)
	{
		mlfw_mat_double_get_dimensions(buffer_matrix_2,&buffer_matrix_2_rows,&buffer_matrix_2_columns);
	}
	if(buffer_matrix_1!=NULL)
	{
		buffer_matrix_1_last_possible_row=number_of_training_examples-(number_of_training_examples%buffer_matrix_1_rows);
	}
	else
	{
		buffer_matrix_1_last_possible_row=0;
	}


	// matrix creation and setting up variables done
	// if target matrix is NULL
	if(*target_matrix==NULL)
	{
		*target_matrix=mlfw_mat_double_create_new(how_many_rows,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
	if(buffers_populated==0)
	{
		buffers_populated=1;
		if(buffer_matrix_2!=NULL)
		{
			f_position=0;
			mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_2,buffer_matrix_1_last_possible_row+1,buffer_matrix_2_rows,&f_position);
			if(mlfw_error()) return;
			mlfw_mat_double_shuffle(buffer_matrix_2,2,buffer_matrix_2_shuffled);
			f_position=0;
			buffer_matrix_2_starts_at_row=buffer_matrix_1_last_possible_row+1;
		}
		if(buffer_matrix_1!=NULL)
		{
			f_position=0;
			mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_1,1,buffer_matrix_1_rows,&f_position);
			if(mlfw_error()) return;
			mlfw_mat_double_shuffle(buffer_matrix_1,2,buffer_matrix_1_shuffled);
			buffer_matrix_1_starts_at_row=1;
			buffer_matrix_1_ends_at_row=buffer_matrix_1_starts_at_row+buffer_matrix_1_rows-1;
		}
	}
	if(from_row>buffer_matrix_1_last_possible_row)
	{
		// serve from buffer_matrix_2_shuffled

		from_buffer_matrix_index=from_row-buffer_matrix_2_starts_at_row;
		to_buffer_matrix_index=from_buffer_matrix_index+how_many_rows-1; // because 1 only
		mlfw_mat_double_copy(*target_matrix,buffer_matrix_2_shuffled,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
		return;
	}
	if(from_row>=buffer_matrix_1_starts_at_row && from_row<=buffer_matrix_1_ends_at_row)
	{
		// data available in buffer_matrix_1_shuffled
		from_buffer_matrix_index=from_row-buffer_matrix_1_starts_at_row;
		to_buffer_matrix_index=from_buffer_matrix_index+how_many_rows-1;
		mlfw_mat_double_copy(*target_matrix,buffer_matrix_1_shuffled,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
		return;
	}

	// buffer matrix 1 reload required
	if(from_row==1)
	{
		f_position=0;
		buffer_matrix_1_starts_at_row=1;
	}
	else
	{
		buffer_matrix_1_starts_at_row=buffer_matrix_1_ends_at_row+1;
	}
	mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_1,buffer_matrix_1_starts_at_row,buffer_matrix_1_rows,&f_position);
	if(mlfw_error()) return;
	mlfw_mat_double_shuffle(buffer_matrix_1,2,buffer_matrix_1_shuffled);
	buffer_matrix_1_ends_at_row=buffer_matrix_1_starts_at_row+buffer_matrix_1_rows-1;
	
	from_buffer_matrix_index=from_row-buffer_matrix_1_starts_at_row;
	to_buffer_matrix_index=from_buffer_matrix_index+how_many_rows-1;
	mlfw_mat_double_copy(*target_matrix,buffer_matrix_1_shuffled,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
	
}


void data_loader(void *x,void *y,uint64_t from_row,uint32_t how_many_rows)
{
	mlfw_mat_double **xxx_matrix=NULL;
	mlfw_column_vec_double **yyy_vector=NULL;
	dimension_t xy_matrix_rows;
	dimension_t xy_matrix_columns;

	double value;
	index_t i;

	xxx_matrix=(mlfw_mat_double **)x;
	yyy_vector=(mlfw_column_vec_double **)y;

	if(x==NULL)
	{
		printf("Data provider error : Feature matrix argument : NULL\n");
		exit(0);
	}
	if(y==NULL)
	{
		printf("Data provider error : Target vector argument : NULL\n");
		exit(0);
	}
	if(how_many_rows==0)
	{
		printf("Data provider error : Number of rows demanded : 0\n");
		exit(0);
	}
	if(from_row>number_of_training_examples)
	{
		*xxx_matrix=NULL;
		*yyy_vector=NULL;
		from_row=1; // reset to begin again from start, for next cycle
		return;
	}
	get_from_file_buffer(&xy_matrix,from_row,how_many_rows);
	mlfw_mat_double_get_dimensions(xy_matrix,&xy_matrix_rows,&xy_matrix_columns);
	if(x_matrix==NULL)
	{
		x_matrix=mlfw_mat_double_create_new(xy_matrix_rows,xy_matrix_columns);
		if(mlfw_error()) return;
		y_vector=mlfw_column_vec_double_create_new(xy_matrix_rows);
		if(mlfw_error()) return;
	}
	mlfw_mat_double_copy(x_matrix,xy_matrix,0,1,0,0,xy_matrix_rows-1,xy_matrix_columns-2);
	
	mlfw_mat_double_fill(x_matrix,0,0,xy_matrix_rows-1,0,1.0);
	for(i=0;i<xy_matrix_rows;++i)
	{
		value=mlfw_mat_double_get(xy_matrix,i,xy_matrix_columns-1);
		mlfw_column_vec_double_set(y_vector,i,value);
	}
	*xxx_matrix=x_matrix;
	*yyy_vector=y_vector;
}
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y,void *model,double regularization_parameter)
{
	
	static uint64_t number_of_training_examples_processed=0;
	static double cost_total=0.0;
	double average;

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
	
	number_of_training_examples_processed++;
	
	y_size=mlfw_column_vec_double_get_size(y);
	model_size=mlfw_column_vec_double_get_size(model);

	if(iteration_number==1)
	{
		// one time act, at the start
		if(SHOW_GRAPH && number_of_training_examples_processed==1)
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
	cost_total=cost_total+cost;
	if(number_of_training_examples_processed==number_of_training_examples)
	{
		average=cost_total/number_of_training_examples;
//		printf("Iteration number %lu, Cost %41.15lf\n",iteration_number,average);
		cost_total=0.0;
	}
	if(SHOW_GRAPH && number_of_training_examples_processed==number_of_training_examples)
	{
		if(gnuplot!=NULL)
		{
			cost_file=fopen("cost.csv","a");
			fprintf(cost_file,"%lu,%f\n",iteration_number,average);
			fclose(cost_file);
		}
	}
	if(number_of_training_examples_processed==number_of_training_examples && (iteration_number%FREQUENCY_OF_PRINTING_COST==0 || iteration_number==NUMBER_OF_ITERATIONS))
	{
		printf("Iteration number %lu, Cost %41.15lf\n",iteration_number,average);
		if(SHOW_GRAPH)
		{
			if(gnuplot!=NULL)
			{
				fprintf(gnuplot,"set term x11 0 position 20,100 title 'Cost Descent'\n");
				fflush(gnuplot);
				fprintf(gnuplot,"set yrange[0:25]\n");
				fflush(gnuplot);
				fprintf(gnuplot,"set xrange[1:%u]\n",NUMBER_OF_ITERATIONS);
				fflush(gnuplot);
				fprintf(gnuplot,"plot 'cost.csv' with lines lw 1 notitle\n");
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
	if(number_of_training_examples_processed==number_of_training_examples)
	{
		number_of_training_examples_processed=0;
	}
	return 0;
err:
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
	mlfw_gradient_descent_options_set_gradient_descent_type(gd_options,MLFW_MINI_BATCH_GRADIENT_DESCENT);
	mlfw_gradient_descent_options_set_progress_callback(gd_options,on_iteration_complete);
	mlfw_gradient_descent_options_set_data_provider(gd_options,data_loader);
	mlfw_gradient_descent_options_set_mini_batch_size(gd_options,MINI_BATCH_SIZE);
	return gd_options;
}
int main()
{
	double regularization_parameter;
	char error_string[512];
	char debug_string[512];
	mlfw_column_vec_double *model=NULL;// for parameters
	mlfw_row_vec_string *model_header=NULL;
	mlfw_gradient_descent_options *gd_options=NULL;

	regularization_parameter=REGULARIZATION_PARAMETER;
	
	init_buffers();
	if(mlfw_error()) goto err;
	gd_options=get_gradient_descent_options(); // function defined by framework user
	if(mlfw_error()) goto err;
	model=mlfw_linear_regression_fit_using_stochastic_gradient_descent(gd_options,regularization_parameter,NULL);
	if(mlfw_error()) goto err;
	model_header=mlfw_row_vec_string_create_new(1);
	if(mlfw_error()) goto err;
	mlfw_row_vec_string_set(model_header,0,"theta");
	if(mlfw_error()) goto err;
	mlfw_column_vec_double_to_csv(model,MODEL_FILE_NAME,model_header);
	if(mlfw_error()) goto err;
	
	printf("Model saved to %s\n",MODEL_FILE_NAME);

	// the globals used by init_buffers
	mlfw_mat_double_destroy(buffer_matrix_1);
	mlfw_mat_double_destroy(buffer_matrix_2);
	mlfw_mat_double_destroy(buffer_matrix_1_shuffled);
	mlfw_mat_double_destroy(buffer_matrix_2_shuffled);

	// the globals used in on_iteration_complete
	mlfw_column_vec_double_destroy(prediction_error);
	mlfw_row_vec_double_destroy(prediction_error_transposed);
	mlfw_column_vec_double_destroy(product_vector);
	mlfw_row_vec_double_destroy(model_transposed);
	mlfw_column_vec_double_destroy(model_squared_sum_vector);

	// the globals used in data_loader
	mlfw_mat_double_destroy(xy_matrix);
	mlfw_mat_double_destroy(x_matrix);
	mlfw_column_vec_double_destroy(y_vector);

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
	
	// the globals used by init_buffers
	mlfw_mat_double_destroy(buffer_matrix_1);
	mlfw_mat_double_destroy(buffer_matrix_2);
	mlfw_mat_double_destroy(buffer_matrix_1_shuffled);
	mlfw_mat_double_destroy(buffer_matrix_2_shuffled);

	// the globals used in on_iteration_complete
	mlfw_column_vec_double_destroy(prediction_error);
	mlfw_row_vec_double_destroy(prediction_error_transposed);
	mlfw_column_vec_double_destroy(product_vector);
	mlfw_row_vec_double_destroy(model_transposed);
	mlfw_column_vec_double_destroy(model_squared_sum_vector);

	// the globals used in data_loader
	mlfw_mat_double_destroy(xy_matrix);
	mlfw_mat_double_destroy(x_matrix);
	mlfw_column_vec_double_destroy(y_vector);

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
