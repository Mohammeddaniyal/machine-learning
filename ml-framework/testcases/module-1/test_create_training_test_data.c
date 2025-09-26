#include<dmlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	dmlfw_row_vec_string *header;
	dmlfw_mat_double *minor_matrix;
	dmlfw_mat_double *major_matrix;

	dimension_t rows,columns;

	dmlfw_mat_double_get_training_testing_data("IceCreamSales.csv",&major_matrix,&minor_matrix,20);
	printf("hi\n");
	if(minor_matrix==NULL || major_matrix==NULL)
	{
		printf("Unable to create testing & training datasets\n");
		return 0;
	}
	dmlfw_mat_double_get_dimensions(minor_matrix,&rows,&columns);
	printf("%d,%d\n",rows,columns);
	dmlfw_mat_double_get_dimensions(major_matrix,&rows,&columns);
	printf("%d,%d\n",rows,columns);
	header=dmlfw_row_vec_string_create_new(2);
	if(header==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	
	dmlfw_row_vec_string_set(header,0,"temperature");
	dmlfw_row_vec_string_set(header,1,"profit");
	printf("Nice\n");
	dmlfw_mat_double_to_csv(minor_matrix,"testing_data.csv",header);
	printf("Dog\n");
	dmlfw_mat_double_to_csv(major_matrix,"training_data.csv",header);
	
	printf("DONE\n");

	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_double_destroy(minor_matrix);
	dmlfw_mat_double_destroy(major_matrix);
	return 0;
}
