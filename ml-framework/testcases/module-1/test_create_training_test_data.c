#include<mlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	mlfw_row_vec_string *header;
	mlfw_mat_double *minor_matrix;
	mlfw_mat_double *major_matrix;

	dimension_t rows,columns;

	mlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&major_matrix,&minor_matrix,20);
	printf("hi\n");
	if(minor_matrix==NULL || major_matrix==NULL)
	{
		printf("Unable to create testing & training datasets\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(minor_matrix,&rows,&columns);
	printf("%d,%d\n",rows,columns);
	mlfw_mat_double_get_dimensions(major_matrix,&rows,&columns);
	printf("%d,%d\n",rows,columns);
	header=mlfw_row_vec_string_create_new(2);
	if(header==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	
	mlfw_row_vec_string_set(header,0,"temperature");
	mlfw_row_vec_string_set(header,1,"profit");
	printf("Nice\n");
	mlfw_mat_double_to_csv(minor_matrix,"testing_data.csv",header);
	printf("Dog\n");
	mlfw_mat_double_to_csv(major_matrix,"training_data.csv",header);
	
	printf("DONE\n");

	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(minor_matrix);
	mlfw_mat_double_destroy(major_matrix);
	return 0;
}
