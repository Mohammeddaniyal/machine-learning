#include<stdio.h>
#include<dmlfw_matrix.h>
#include<dmlfw_operations.h>
int main()
{
	dmlfw_mat_double *matrix;
	dmlfw_mat_double *inverse_matrix;
	dmlfw_mat_double *product_matrix;
	dimension_t rows,columns;
	index_t r,c;
	matrix=dmlfw_mat_double_create_new(3,3);
	if(matrix==NULL)
	{
		printf("Unable to create matrix\n");
		return 0;
	}
	dmlfw_mat_double_set(matrix,0,0,1);
	dmlfw_mat_double_set(matrix,0,1,2);
	dmlfw_mat_double_set(matrix,0,2,3);
	dmlfw_mat_double_set(matrix,1,0,3);
	dmlfw_mat_double_set(matrix,1,1,2);
	dmlfw_mat_double_set(matrix,1,2,1);
	dmlfw_mat_double_set(matrix,2,0,1);
	dmlfw_mat_double_set(matrix,2,1,3);
	dmlfw_mat_double_set(matrix,2,2,2);
	
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	inverse_matrix=dmlfw_mat_double_inverse(matrix,NULL);
	if(inverse_matrix==NULL)
	{
		printf("Unable to create inverse matrix\n");
		dmlfw_mat_double_destroy(matrix);
		return 0;
	}

	dmlfw_mat_double_get_dimensions(inverse_matrix,&rows,&columns);
	
	printf("Matrix to inverse\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	printf("Inversed matrix\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(inverse_matrix,r,c));
		}
		printf("\n");
	}

	product_matrix=dmlfw_multiply_double_matrix_with_matrix(matrix,inverse_matrix,NULL);
	if(product_matrix==NULL)
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_mat_double_destroy(inverse_matrix);
		return 0;
	}

	printf("Product Matrix\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(product_matrix,r,c));
		}
		printf("\n");
	}
	dmlfw_mat_double_destroy(matrix);
	dmlfw_mat_double_destroy(inverse_matrix);
	dmlfw_mat_double_destroy(product_matrix);
	return 0;
}
