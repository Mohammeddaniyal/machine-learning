#include<stdio.h>
#include<mlfw_matrix.h>
int main()
{
	mlfw_mat_double *matrix;
	mlfw_mat_double *inverse_matrix;
	dimension_t rows,columns;
	index_t r,c;
	matrix=mlfw_mat_double_create_new(3,3);
	if(matrix==NULL)
	{
		printf("Unable to create matrix\n");
		return 0;
	}
	mlfw_mat_double_set(matrix,0,0,1);
	mlfw_mat_double_set(matrix,0,1,2);
	mlfw_mat_double_set(matrix,0,2,3);
	mlfw_mat_double_set(matrix,1,0,3);
	mlfw_mat_double_set(matrix,1,1,2);
	mlfw_mat_double_set(matrix,1,2,1);
	mlfw_mat_double_set(matrix,2,0,1);
	mlfw_mat_double_set(matrix,2,1,3);
	mlfw_mat_double_set(matrix,2,2,2);
	
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
	inverse_matrix=mlfw_mat_double_inverse(matrix,NULL);
	if(inverse_matrix==NULL)
	{
		printf("Unable to create inverse matrix\n");
		mlfw_mat_double_destroy(matrix);
		return 0;
	}

	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(inverse_matrix,r,c));
		}
		printf("\n");
	}

	mlfw_mat_double_destroy(matrix);
	mlfw_mat_double_destroy(inverse_matrix);
	return 0;
}
