#include<stdio.h>
#include<dmlfw_matrix.h>
#include<dmlfw_scale.h>
#include<dmlfw_vector.h>
int main()
{
	dmlfw_mat_double *scaled_matrix;
	dmlfw_row_vec_string *columns_to_scale;
	dmlfw_row_vec_string *matrix_header;
	columns_to_scale=dmlfw_row_vec_string_create_new(2);
	if(columns_to_scale==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_row_vec_string_set(columns_to_scale,0,"age");
	dmlfw_row_vec_string_set(columns_to_scale,1,"salary");
	scaled_matrix=dmlfw_scale_double("dummy.csv",columns_to_scale,"mean_sd.csv","z-score",NULL);
	if(scaled_matrix==NULL)
	{
		printf("Low memory or some other reason, scaling not done\n");
		dmlfw_row_vec_string_destroy(columns_to_scale);
		return 0;
	}
	matrix_header=dmlfw_row_vec_string_create_new(4);
	if(matrix_header==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_string_destroy(columns_to_scale);
		dmlfw_mat_double_destroy(scaled_matrix);
		return 0;
	}
	dmlfw_row_vec_string_set(matrix_header,0,"age");
	dmlfw_row_vec_string_set(matrix_header,1,"height");
	dmlfw_row_vec_string_set(matrix_header,2,"salary");
	dmlfw_row_vec_string_set(matrix_header,3,"incentive");
	dmlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy.csv",matrix_header);
	
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	return 0;
}
