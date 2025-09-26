#include<stdio.h>
#include<dmlfw.h>
int main(int argc,char *argv[])
{
	dmlfw_mat_double *scaled_matrix=NULL;
	dmlfw_mat_double *matrix=NULL;
	dmlfw_row_vec_string *matrix_header=NULL;
	char *algorithm;
	char *parameters_file;
	char *source;
	char *target;
	char error_string[512];
	
	if(argc!=5)
	{
		printf("Specify [source_csv] [target_csv] [algorithm] [parameters_csv]\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	algorithm=argv[3];
	parameters_file=argv[4];
	matrix=dmlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(dmlfw_error()) goto err;
	dmlfw_mat_double_destroy(matrix);
	matrix=NULL;
	if(dmlfw_error()) goto err;	
	scaled_matrix=dmlfw_scale_double_with_given_parameters(source,parameters_file,algorithm,NULL);
	if(dmlfw_error()) goto err;

	dmlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	printf("%s file generated\n",target);
	if(dmlfw_error()) goto err;	
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
err:
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
}
