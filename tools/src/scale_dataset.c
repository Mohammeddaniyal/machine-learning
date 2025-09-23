#include<stdio.h>
#include<mlfw.h>
int main(int argc,char *argv[])
{
	mlfw_mat_double *scaled_matrix=NULL;
	mlfw_row_vec_string *columns_to_scale=NULL;
	mlfw_mat_double *matrix=NULL;
	mlfw_row_vec_string *matrix_header=NULL;
	char *algorithm;
	char *parameters_file;
	char *source;
	char *target;
	char error_string[512];
	index_t i;
	dimension_t columns_to_scale_size;
	
	if(argc<=5)
	{
		printf("Specify [source csv] [target csv] [algorithm] [parameters_csv] columns_to_scale ....\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	algorithm=argv[3];
	parameters_file=argv[4];
	columns_to_scale_size=argc-5; // -5 because (program_name,source,target,algorithm and parameters_csv)
	matrix=mlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(mlfw_error()) goto err;
	mlfw_mat_double_destroy(matrix);
	matrix=NULL;
	columns_to_scale=mlfw_row_vec_string_create_new(columns_to_scale_size);
	if(mlfw_error()) goto err;	
	for(i=5;i<argc;++i)
	{
		mlfw_row_vec_string_set(columns_to_scale,i-5,argv[i]);
	}
	scaled_matrix=mlfw_scale_double(source,columns_to_scale,parameters_file,algorithm,NULL);
	if(mlfw_error()) goto err;

	mlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	
	mlfw_row_vec_string_destroy(columns_to_scale);
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_mat_double_destroy(matrix);
	return 0;
err:
	mlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	mlfw_row_vec_string_destroy(columns_to_scale);
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_row_vec_string_destroy(matrix_header);
	mlfw_mat_double_destroy(matrix);
	return 0;
}
