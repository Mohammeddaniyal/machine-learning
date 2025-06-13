#include<mlfw_matrix.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *matrix;
	dimension_t rows,columns;
	index_t r,c;

	matrix=mlfw_mat_double_create_new(2,3);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_mat_double_set(matrix,0,0,10.33);
	mlfw_mat_double_set(matrix,0,1,20.33);
	mlfw_mat_double_set(matrix,0,2,30.33);
	mlfw_mat_double_set(matrix,1,0,100.33);
	mlfw_mat_double_set(matrix,1,1,200.33);
	mlfw_mat_double_set(matrix,1,2,300.33);


	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	printf("Before right shift\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));		
		}
		printf("\n");
	}

	mlfw_mat_double_reshape(matrix,1);
	printf("After applying right shift\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));		
		}
		printf("\n");
	}
	mlfw_mat_double_destroy(matrix);
	return 0;
}
