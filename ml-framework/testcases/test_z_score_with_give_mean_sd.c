#include<stdio.h>
#include<mlfw_scale.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
int main()
{
	mlfw_row_vec_string *header,*mean_sd_header;
	mlfw_mat_double *matrix,*scaled_matrix,*mean_sd_matrix;
	dimension_t rows,columns;
	matrix=mlfw_mat_double_from_csv("dummy.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load dummy.csv\n");
		return 0;
	}
	mean_sd_matrix=mlfw_mat_double_from_csv("mean_sd.csv",NULL,&mean_sd_header);
	if(mean_sd_matrix==NULL)
	{
		printf("Unable to load mean_sd.csv\n");
		mlfw_mat_double_destroy(matrix);
		return 0;

	}
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	scaled_matrix=mlfw_scale_double_z_score_with_given_mean_standard_deviation(matrix,0,0,rows-1,columns-1,mean_sd_matrix,NULL);
	mlfw_mat_double_to_csv(scaled_matrix,"scaled_dummy2.csv",header);
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(scaled_matrix);
	mlfw_mat_double_destroy(mean_sd_matrix);
	mlfw_row_vec_string_destroy(mean_sd_header);
	return 0;
}
