#include<dmlfw_matrix.h>
#include<stdio.h>
#include<stdlib.h>
int main(int args,char *argv[])
{
	dmlfw_row_vec_string *header;

	dmlfw_mat_string *matrix;
	dmlfw_mat_string *shuffled_matrix;
	
	dmlfw_mat_string *minor_matrix;
	dmlfw_mat_string *major_matrix;
	
	dimension_t shuffled_matrix_rows;
	dimension_t shuffled_matrix_columns;
	dimension_t minor_rows;
	dimension_t major_rows;
	char *dataset_file;
	char *test_file;
	char *train_file;
	int minor_percentage;
	// as arguments (dataset file, test_file, train_file, minor_percentage)
	if(args!=5)
	{
		printf("Usage : [create_test_train_dataset dataset test_file train_file minor_percentage]\n");
		return 0;
	}
	dataset_file=argv[1];
	test_file=argv[2];
	train_file=argv[3];
	minor_percentage=atoi(argv[4]);


	if(minor_percentage<=0 || minor_percentage>50)
	{
		printf("Invalid minor percentage %d\n",minor_percentage);
		return 0;
	}
	matrix=dmlfw_mat_string_from_csv(dataset_file,NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load %s\n",dataset_file);
		return 0;
	}
	shuffled_matrix=dmlfw_mat_string_shuffle(matrix,3,NULL); // shuffle 3 times
	if(shuffled_matrix==NULL)
	{
		printf("Unable to create test/train data file\n");
		dmlfw_mat_string_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		return 0;
	}
	dmlfw_mat_string_get_dimensions(shuffled_matrix,&shuffled_matrix_rows,&shuffled_matrix_columns);
	
	minor_rows=(minor_percentage*shuffled_matrix_rows)/100;
	major_rows=shuffled_matrix_rows-minor_rows;
	
	minor_matrix=dmlfw_mat_string_create_new(minor_rows,shuffled_matrix_columns);
	if(minor_matrix==NULL)
	{
		printf("Unable to load the test/train data file\n");
		dmlfw_mat_string_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_string_destroy(shuffled_matrix);
		return 0;
	}
	major_matrix=dmlfw_mat_string_create_new(major_rows,shuffled_matrix_columns);
	if(major_matrix==NULL)
	{
		printf("Unable to load the test/train data file\n");
		dmlfw_mat_string_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_string_destroy(shuffled_matrix);
		dmlfw_mat_string_destroy(minor_matrix);
		return 0;
	}
	
dmlfw_mat_string_copy(minor_matrix,shuffled_matrix,0,0,0,0,minor_rows-1,shuffled_matrix_columns-1);
dmlfw_mat_string_copy(major_matrix,shuffled_matrix,0,0,minor_rows,0,shuffled_matrix_rows-1,shuffled_matrix_columns-1);

	dmlfw_mat_string_to_csv(minor_matrix,test_file,header);
	dmlfw_mat_string_to_csv(major_matrix,train_file,header);

	dmlfw_mat_string_destroy(matrix);
	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_string_destroy(shuffled_matrix);
	dmlfw_mat_string_destroy(minor_matrix);
	dmlfw_mat_string_destroy(major_matrix);
	return 0;
}
