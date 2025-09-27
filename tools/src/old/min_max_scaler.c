#include<stdio.h>
#include<stdlib.h>
#include<dmlfw_scale.h>
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
int main(int argc,char *argv[])
{

	dmlfw_mat_double *matrix;
	dmlfw_mat_double *scaled_matrix;
	dmlfw_row_vec_string *header;
	dimension_t matrix_rows,matrix_columns;

	char *input_file;
	char *output_file;
	char *min_max_file;
	int from_column_index;
	int to_column_index;
	if(argc!=6)
	{
		printf("Usage : [min_max_scaler.out input_file output_file from_column to_column min_max_file]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	from_column_index=atoi(argv[3]);
	to_column_index=atoi(argv[4]);
	min_max_file=argv[5];

	matrix=dmlfw_mat_double_from_csv(input_file,NULL,&header);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	
	scaled_matrix=dmlfw_scale_double_min_max(matrix,0,from_column_index,matrix_rows-1,to_column_index,min_max_file,NULL);
	if(scaled_matrix==NULL)
	{
		printf("Low memory, while creating scaled matrix\n");
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		return 0;
	}
	
	dmlfw_mat_double_to_csv(scaled_matrix,output_file,header);
		
	dmlfw_mat_double_destroy(matrix);
	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_double_destroy(scaled_matrix);
	return 0;
}

/** @} */
