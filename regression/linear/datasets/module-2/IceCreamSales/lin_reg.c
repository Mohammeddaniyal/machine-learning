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
	mlfw_mat_double *matrix;
	mlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;
	mlfw_column_vec_double *target_values_vector;
	mlfw_row_vec_double *trained_parameters;
	dimension_t trained_parameters_size;
	index_t i;
	if(argc!=2)
	{
		printf("Pass numbers of iteration as command line argument\n");
		return 0;
	}
	matrix=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&matrix_header);
	if(matrix==NULL)
	{
		printf("Unable to load IceCreamSales.csv\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	target_values_vector=mlfw_mat_double_create_column_vec(matrix,1,NULL);
	if(target_values_vector==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	mlfw_mat_double_reshape(&matrix,matrix_rows,matrix_columns-1);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		mlfw_column_vec_double_destroy(target_values_vector);
		return 0;
	}
	matrix_columns=matrix_columns-1;

	trained_parameters=mlfw_linear_regression_gradient_descent_fit_line(matrix,target_values_vector,0.0003,atoi(argv[1]),screen_logger);
	if(trained_parameters==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		mlfw_column_vec_double_destroy(target_values_vector);
		return 0;
	}
	trained_parameters_size=mlfw_row_vec_double_get_size(trained_parameters);
	for(i=0;i<trained_parameters_size;++i)
	{
		printf("%lf ",mlfw_row_vec_double_get(trained_parameters,i));
	}
	
	
	mlfw_mat_double_destroy(matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_column_vec_double_destroy(target_values_vector);
	mlfw_row_vec_double_destroy(trained_parameters);

	return 0;
}
