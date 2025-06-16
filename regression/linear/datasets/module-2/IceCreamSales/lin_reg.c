#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_learning.h>
#include<stdlib.h>
#include<stdio.h>
uint8_t screen_logger(uint64_t iteration_number,double error_value)
{
	printf("Iteration : %" PRIu64 ",Error : %40.15lf\n",iteration_number,error_value);
	return 1;// keep running
}
int main(int argc,char *argv[])
{
	mlfw_mat_double *training_examples_matrix;
	dimension_t training_examples_matrix_rows,training_examples_matrix_columns;
	mlfw_column_vec_double *training_examples_target_values_vector;
	mlfw_row_vec_double *trained_parameters;
	dimension_t trained_parameters_size;
	mlfw_mat_double *test_examples_matrix;
	mlfw_column_vec_double *test_examples_target_values_vector;
	dimension_t test_examples_matrix_rows,test_examples_matrix_columns;
	mlfw_column_vec_double *test_examples_predicted_values_vector;

	index_t i;
	if(argc!=2)
	{
		printf("Pass numbers of iteration as command line argument\n");
		return 0;
	}
		
	
	mlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&training_examples_matrix,&test_examples_matrix,20);
	if(training_examples_matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}


	mlfw_mat_double_get_dimensions(training_examples_matrix,&training_examples_matrix_rows,&training_examples_matrix_columns);
	mlfw_mat_double_get_dimensions(test_examples_matrix,&test_examples_matrix_rows,&test_examples_matrix_columns);
	training_examples_target_values_vector=mlfw_mat_double_create_column_vec(training_examples_matrix,1,NULL);
	if(training_examples_target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		return 0;
	}
	mlfw_mat_double_reshape(&training_examples_matrix,training_examples_matrix_rows,training_examples_matrix_columns-1);
	if(training_examples_matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
		mlfw_column_vec_double_destroy(training_examples_target_values_vector);
		return 0;
	}
	training_examples_matrix_columns=training_examples_matrix_columns-1;

	trained_parameters=mlfw_linear_regression_gradient_descent_fit_line(training_examples_matrix,training_examples_target_values_vector,0.0003,atoi(argv[1]),screen_logger);
	if(trained_parameters==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(training_examples_matrix);
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




	printf("Result is (0-1) : %40.15lf \n",mlfw_get_r2_score(test_examples_target_values_vector,test_examples_predicted_values_vector));
	
	mlfw_row_vec_double_destroy(trained_parameters);
	mlfw_column_vec_double_destroy(test_examples_target_values_vector);
	mlfw_mat_double_destroy(test_examples_matrix);

	return 0;
}
