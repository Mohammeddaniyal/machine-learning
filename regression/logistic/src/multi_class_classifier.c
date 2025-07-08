#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_learning.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
#include<stdio_ext.h>
#include<unistd.h>

uint8_t KEEP_RUNNING=1;


struct thread_args
{
mlfw_mat_double *matrix;
mlfw_column_vec_double *target_values_vector;
double learning_rate;
uint64_t iteration_number;
uint8_t (*callback)(uint64_t,double);
mlfw_set_string *class_set;
};

uint8_t screen_logger(uint64_t iteration_number,double cost)
{

	printf("Iteration : %" PRIu64 ",Error : %40.15lf\n",iteration_number,cost);
	return KEEP_RUNNING;// keep running
}

void * thread_function(void *d)
{
mlfw_mat_double *trained_parameters_matrix;
struct thread_args *args;
args=(struct thread_args *)d;

trained_parameters_matrix=mlfw_logistic_regression_gradient_descent_fit(args->matrix,args->target_values_vector,args->class_set,args->learning_rate,args->iteration_number,args->callback);

sleep(1); // just so that even after the fit line ends, the thread should not end immediately

return (void *)trained_parameters_matrix;

}

int main(int argc,char *argv[])
{
	pthread_t thread_id;
	struct thread_args wrapper;
	mlfw_set_string *class_set;
	dimension_t class_set_size;
	mlfw_mat_double *training_examples_matrix;
	dimension_t training_examples_matrix_rows,training_examples_matrix_columns;
	mlfw_column_vec_double *training_examples_target_values_vector;
	mlfw_mat_double *trained_parameters_matrix;
	dimension_t trained_parameters_matrix_rows,trained_parameter_matrix_columns;
	mlfw_mat_double *test_examples_matrix;
	mlfw_column_vec_double *test_examples_target_values_vector;
	dimension_t test_examples_matrix_rows,test_examples_matrix_columns;
	mlfw_column_vec_double *test_examples_predicted_values_vector;
	double r2_score;
	index_t i;
	

	char *end;
	char *dataset_name;
	double learning_rate;
//	int test_data_percentage;
	uint64_t number_of_iterations;
	char *model_csv_name; // to store trained parameters
	mlfw_row_vec_string *trained_parameters_header;
	char str[11];
	char *str_ptr;
	mlfw_row_vec_string *header;
	
	if(argc!=6)
	{
		printf("Usage [multi_class_classifier dataset.csv learning_rate test_data_percentage number_of_iterations model_csv_name]\n");
		return 0;
	}
	
	dataset_name=argv[1];
	learning_rate=strtod(argv[2],&end);
//	test_data_percentage=atoi(argv[3]);
	number_of_iterations=atoi(argv[4]);
	model_csv_name=argv[5];	
	
	class_set=mlfw_set_string_create_new();
	if(class_set==NULL)
	{
		printf("Low memory\n");
		return 0;
	}

	//mlfw_mat_double_get_training_testing_data(dataset_name,&training_examples_matrix,&test_examples_matrix,test_data_percentage);
	training_examples_matrix=mlfw_mat_double_from_csv(dataset_name,NULL,&header);
	if(training_examples_matrix==NULL)
	{
		printf("Unable to load %s\n",dataset_name);
		mlfw_set_string_destroy(class_set);
		return 0;
	}
	mlfw_row_vec_string_destroy(header);
	test_examples_matrix=mlfw_mat_double_from_csv(dataset_name,NULL,&header);
	if(test_examples_matrix==NULL)
	{
		printf("Unable to load %s\n",dataset_name);
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(training_examples_matrix);
		return 0;
	}
	mlfw_row_vec_string_destroy(header);

	mlfw_mat_double_get_dimensions(training_examples_matrix,&training_examples_matrix_rows,&training_examples_matrix_columns);
	mlfw_mat_double_get_dimensions(test_examples_matrix,&test_examples_matrix_rows,&test_examples_matrix_columns);
	training_examples_target_values_vector=mlfw_mat_double_create_column_vec(training_examples_matrix,training_examples_matrix_columns-1,NULL);
	if(training_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		return 0;
	}
	mlfw_mat_double_reshape(&training_examples_matrix,training_examples_matrix_rows,training_examples_matrix_columns-1);
	if(training_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	training_examples_matrix_columns=training_examples_matrix_columns-1;

	wrapper.matrix=training_examples_matrix;
	wrapper.target_values_vector=training_examples_target_values_vector;
	wrapper.learning_rate=learning_rate;
	wrapper.iteration_number=number_of_iterations;
	wrapper.callback=screen_logger;
	wrapper.class_set=class_set;
	pthread_create(&thread_id,NULL,thread_function,(void *)&wrapper);
	
	// this below line is very very very very important
	pthread_join(thread_id,(void **)&trained_parameters_matrix);

	if(trained_parameters_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	mlfw_mat_double_destroy(training_examples_matrix);
	mlfw_column_vec_double_destroy(training_examples_target_values_vector);
	
	// training parts ends here
	
	mlfw_mat_double_get_dimensions(trained_parameters_matrix,&trained_parameters_matrix_rows,&trained_parameters_matrix_columns);

	
	// prediction part starts here
	

	test_examples_target_values_vector=mlfw_mat_double_create_column_vec(test_examples_matrix,test_examples_matrix_columns-1,NULL);
	if(test_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_mat_double_destroy(trained_parameters_matrix);
		return 0;
	}

	mlfw_mat_double_reshape(&test_examples_matrix,test_examples_matrix_rows,test_examples_matrix_columns-1);
	if(test_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_row_vec_double_destroy(trained_parameters);
		return 0;
	}
	test_examples_matrix_columns=test_examples_matrix_columns-1;
	test_examples_predicted_values_vector=mlfw_linear_regression_multi_class_predict(test_examples_matrix,trained_parameters_matrix);
	if(test_examples_predicted_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(class_set);
		mlfw_mat_double_destroy(test_examples_matrix);
		mlfw_column_vec_double_destroy(test_examples_target_values_vector);
		mlfw_mat_double_destroy(trained_parameters_matrix);
		return 0;
	}
	
	// prediction part ends here

	r2_score=mlfw_get_r2_score(test_examples_target_values_vector,test_examples_predicted_values_vector);
	printf("Good accuracy score is anything greater than or equal to 0.7\n");

	printf("Accuracy score (0-1) is : %lf \n",r2_score);

	class_set_size=mlfw_set_string_get_size(class_set);
	trained_parameters_matrix_header=mlfw_row_vec_string_create_new(class_set_size);
	if(trained_parameters_matrix_header==NULL)
	{
		printf("Unable to create model file %s\n",model_csv_name);	
	}
	else
	{
		for(i=0;i<class_set_size;++i)
		{
			mlfw_set_string_get(class_set,i,&str_ptr);
			if(str_ptr==NULL)
			{
			mlfw_set_string_destroy(class_set);
			mlfw_mat_double_destroy(trained_parameters_matrix);
			mlfw_row_vec_string_destroy(trained_parameters_header);
			mlfw_mat_double_destroy(test_examples_matrix);
			mlfw_column_vec_double_destroy(test_examples_target_values_vector);
			mlfw_column_vec_double_destroy(test_examples_predicted_values_vector);
			return 0;
			}
			mlfw_row_vec_string_set(trained_parameters_matrix_header,i,str_ptr);
			free(str_ptr);
		}
		mlfw_row_vec_double_to_csv(trained_parameters,model_csv_name,trained_parameters_matrix_header);
		printf("Model %s created\n",model_csv_name);
	}	
	mlfw_set_string_destroy(class_set);
	mlfw_mat_double_destroy(trained_parameters_matrix);
	mlfw_row_vec_string_destroy(trained_parameters_matrix_header);
	mlfw_mat_double_destroy(test_examples_matrix);
	mlfw_column_vec_double_destroy(test_examples_target_values_vector);
	mlfw_column_vec_double_destroy(test_examples_predicted_values_vector);

	return 0;
}
