#include<stdio.h>
#include<dmlfw_scale.h>
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
int main()
{
	dmlfw_row_vec_string *header;
	dmlfw_mat_double *matrix,*scaled_matrix;
	dimension_t rows,columns;
	matrix=dmlfw_mat_double_from_csv("dummy.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load dummy.csv\n");
		return 0;
	}
	dmlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	scaled_matrix=dmlfw_scale_double_z_score(matrix,0,0,rows-1,columns-1,"mean_sd.csv",NULL);
	dmlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy.csv",header);
	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_double_destroy(matrix);
	dmlfw_mat_double_destroy(scaled_matrix);
	return 0;
}
