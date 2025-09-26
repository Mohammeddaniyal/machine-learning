#include<dmlfw_matrix.h>
int main()
{
	dmlfw_mat_string *matrix;
	matrix=dmlfw_mat_string_create_new(2,3);
	dmlfw_mat_string_set(matrix,0,0,"One");
	dmlfw_mat_string_set(matrix,0,1,"Two");
	dmlfw_mat_string_set(matrix,0,2,"Three");
	dmlfw_mat_string_set(matrix,1,0,"Four");
	dmlfw_mat_string_set(matrix,1,1,"Five");
	dmlfw_mat_string_set(matrix,1,2,"Six");

	dmlfw_mat_string_to_csv(matrix,"matrix.csv");

	dmlfw_mat_string_destroy(matrix);
	return 0;
}
