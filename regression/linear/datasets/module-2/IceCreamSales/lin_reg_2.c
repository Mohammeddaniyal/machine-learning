#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_learning.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>

uint8_t KEEP_RUNNING=1;


struct thread_args
{
mlfw_mat_double *matrix;
mlfw_column_vec_double *target_values_vector;
double learning_rate;
uint64_t iteration_number;
uint8_t (*callback)(uint64_t,double);
};

uint8_t screen_logger(uint64_t iteration_number,double error_value)
{
	printf("Iteration : %" PRIu64 ",Error : %40.15lf\n",iteration_number,error_value);
	return KEEP_RUNNING;// keep running
}

void * thread_function(void *d)
{
mlfw_row_vec_double *trained_parameters;
struct thread_args *args;
args=(struct thread_args *)d;

trained_parameters=mlfw_linear_regression_gradient_descent_fit_line(args->matrix,args->target_values_vectorr,args->learning_rate,args->iteration_number,args->callback);

sleep(1); // just so that even after the fit line ends, the thread should not end immediately

return (void *)trained_parameter;

}

int main(int argc,char *argv[])
{
	pthread_t thread_id;
	char m;
	struct thread_args wrapper;
	mlfw_mat_double *training_examples_matrix;
	dimension_t training_examples_matrix_rows,training_examples_matrix_columns;
	mlfw_column_vec_double *training_examples_target_values_vector;
	mlfw_row_vec_double *trained_parameters;
	dimension_t trained_parameters_size;
	mlfw_mat_double *test_examples_matrix;
	mlfw_column_vec_double *test_examples_target_values_vector;
	dimension_t test_examples_matrix_rows,test_examples_matrix_columns;
	mlfw_column_vec_double *test_examples_predicted_values_vector;
	double r2_score;
	index_t i;
	if(argc!=2)
	{
		printf("Pass numbers of iteration as command line argument\n");
		return 0;
	}
		
	
	mlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&training_examples_matrix,&test_examples_matrix,20);
	if(training_examples_matrix==NULL)
	{
		printf("Unable to load IceCreamSale.csv\n");
		return 0;
	}


	mlfw_mat_double_get_dimensions(training_examples_matrix,&training_examples_matrix_rows,&training_examples_matrix_columns);
	mlfw_mat_double_get_dimensions(test_examples_matrix,&test_examples_matrix_rows,&test_examples_matrix_columns);
	training_examples_target_values_vector=mlfw_mat_double_create_column_vec(training_examples_matrix,1,NULL);
	if(training_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		return 0;
	}
	mlfw_mat_double_reshape(&training_examples_matrix,training_examples_matrix_rows,training_examples_matrix_columns-1);
	if(training_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	training_examples_matrix_columns=training_examples_matrix_columns-1;

	wrapper.matrix=training_examples_matrix;
	wrapper.target_values_vector=training_examples_target_values_vector;
	wrapper.learning_rate=0.0003;
	wrapper.iteration_number=atoi(argv[1]);
	wrapper.callback=screen_logger;
	pthread_create(&thread_id,NULL,thread_function,(void *)&wrapper);
	while(1)
	{
		m=getchar();
		__fpurge(stdin);
		if(m=='\n') 
		{
			KEEP_RUNNING=0;
			break;
		}
	}
	
	// this below line is very very very very important
	pthread_join(&thread_id,(void **)&trained_parameters);

	if(trained_parameters==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	mlfw_mat_double_destroy(training_examples_matrix);
	mlfw_column_vec_double_destroy(training_examples_target_values_vector);
	
	// training parts ends here
	
	trained_parameters_size=mlfw_row_vec_double_get_size(trained_parameters);
	for(i=0;i<trained_parameters_size;++i)
	{
		printf("%lf ",mlfw_row_vec_double_get(trained_parameters,i));
	}
	
		printf("\n");
	// prediction part starts here
	

	test_examples_target_values_vector=mlfw_mat_double_create_column_vec(test_examples_matrix,1,NULL);
	if(test_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}

	mlfw_mat_double_reshape(&test_examples_matrix,test_examples_matrix_rows,test_examples_matrix_columns-1);
	if(test_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	test_examples_matrix_columns=test_examples_matrix_columns-1;
	test_examples_predicted_values_vector=mlfw_linear_regression_predict(test_examples_matrix,trained_parameters);
	if(test_examples_predicted_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(test_examples_target_values_vector);
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	
	// prediction part ends here

	r2_score=mlfw_get_r2_score(test_examples_target_values_vector,test_examples_predicted_values_vector);
	printf("Good accuracy score is anything greater than or equal to 0.7\n)");

	printf("Accuracy score (0-1) is : %lf \n",r2_score);
	
	mlfw_row_vec_double_destroy(trained_parameters);
	mlfw_mat_double_destroy(test_examples_matrix);
	mlfw_column_vec_double_destroy(test_examples_target_values_vector);
	mlfw_column_vec_double_destroy(test_examples_predicted_values_vector);

	return 0;
}
