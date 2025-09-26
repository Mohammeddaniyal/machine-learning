#include<stdio.h>
#include<dmlfw_matrix.h>
#include<dmlfw_scale.h>
#include<dmlfw_vector.h>
int main(int argc,char *argv[])
{
	dmlfw_mat_double *scaled_matrix;
	dmlfw_row_vec_string *columns_to_scale;
	dmlfw_mat_double *matrix;
	dmlfw_row_vec_string *matrix_header;
	char *source;
	char *target;
	index_t i;
	dimension_t columns_to_scale_size;
	
	if(argc<=3)
	{
		printf("Specify [source csv] [target csv] columns_to_scale ....\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	matrix=dmlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(matrix==NULL)
	{
		printf("Unable to load %s\n",source);
		return 0;
	}
	dmlfw_mat_double_destroy(matrix);
	columns_to_scale_size=argc-3; // -3 because (program_name,source,target)
	columns_to_scale=dmlfw_row_vec_string_create_new(columns_to_scale_size);
	if(columns_to_scale==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	
	for(i=3;i<argc;++i)
	{
		dmlfw_row_vec_string_set(columns_to_scale,i-3,argv[i]);
	}
	scaled_matrix=dmlfw_scale_double(source,columns_to_scale,"z_score.csv","z-score",NULL);
	if(scaled_matrix==NULL)
	{
		printf("Low memory or some other reason, scaling not done\n");
		dmlfw_row_vec_string_destroy(columns_to_scale);
		dmlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	dmlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	return 0;
}
