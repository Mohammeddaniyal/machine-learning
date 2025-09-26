#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
void test_new_and_destroy()
{
	index_t r,c;
	double value;
	dmlfw_mat_double *matrix;
	dimension_t rows;
	dimension_t columns;
	printf("Test case for new and destroy\n");
	matrix=dmlfw_mat_double_create_new(3,4);
	if(matrix==NULL)
	{
		printf("Unable to create matrix, low memory\n");
		return;
	}
	value=10.23;
	dmlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			dmlfw_mat_double_set(matrix,r,c,value);
			value+=10.20;
		}
	}
	printf("Contents of matrix\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%20.10lf ",dmlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	dmlfw_mat_double_destroy(matrix);
}
int main()
{
	test_new_and_destroy();
	return 0;
}
