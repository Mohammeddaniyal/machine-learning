#include<stdio.h>
#include<mlfw.h>
int main(int argc,char *argv[])
{
	mlfw_mat_double *scaled_matrix=NULL;
	mlfw_mat_double *matrix=NULL;
	mlfw_row_vec_string *matrix_header=NULL;
	char *algorithm;
	char *parameters_file;
	char *source;
	char *target;
	char error_string[512];
	
	if(argc<=5)
	{
		printf("Specify [source csv] [target csv] [algorithm] [parameters_csv] columns_to_scale ....\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	algorithm=argv[3];
	parameters_file=argv[4];
	matrix=mlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(mlfw_error()) goto err;
	mlfw_mat_double_destroy(matrix);
	matrix=NULL;
	if(mlfw_error()) goto err;	
	scaled_matrix=mlfw_scale_double_with_given_parameters(source,parameters_file,algorithm,NULL);
	if(mlfw_error()) goto err;

	mlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_mat_double_destroy(matrix);
	return 0;
err:
	mlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_mat_double_destroy(matrix);
	return 0;
}
