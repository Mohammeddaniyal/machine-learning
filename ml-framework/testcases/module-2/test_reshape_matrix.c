#include<dmlfw_matrix.h>
#include<stdio.h>
int main()
{
	dmlfw_mat_double *matrix;
	dimension_t rows,columns;
	index_t r,c;

	matrix=dmlfw_mat_double_create_new(2,3);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_mat_double_set(matrix,0,0,10.33);
	dmlfw_mat_double_set(matrix,0,1,20.33);
	dmlfw_mat_double_set(matrix,0,2,30.33);
	dmlfw_mat_double_set(matrix,1,0,100.33);
	dmlfw_mat_double_set(matrix,1,1,200.33);
	dmlfw_mat_double_set(matrix,1,2,300.33);


	dmlfw_mat_double_reshape(&matrix,1,2);
	if(matrix==NULL)
	{
		printf("Memory issue\n");
		return 0;
	}
	dmlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(matrix,r,c));		
		}
		printf("\n");
	}

	dmlfw_mat_double_destroy(matrix);
	return 0;
}
