#include<stdio.h>
#include<dmlfw.h>
int main(int argc,char *argv[])
{
	dmlfw_mat_double *scaled_matrix=NULL;
	dmlfw_row_vec_string *columns_to_scale=NULL;
	dmlfw_mat_double *matrix=NULL;
	dmlfw_row_vec_string *matrix_header=NULL;
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
	matrix=dmlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(dmlfw_error()) goto err;
	dmlfw_mat_double_destroy(matrix);
	matrix=NULL;
	columns_to_scale=dmlfw_row_vec_string_create_new(columns_to_scale_size);
	if(dmlfw_error()) goto err;	
	for(i=5;i<argc;++i)
	{
		dmlfw_row_vec_string_set(columns_to_scale,i-5,argv[i]);
	}
	scaled_matrix=dmlfw_scale_double(source,columns_to_scale,parameters_file,algorithm,NULL);
	if(dmlfw_error()) goto err;

	dmlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
err:
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
}
