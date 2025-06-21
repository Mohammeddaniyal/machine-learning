#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
void test_from_csv()
{
	index_t r,c;
	mlfw_mat_double *matrix;
	dimension_t rows,columns;
	printf("Test case for from csv\n");
	matrix=mlfw_mat_double_from_csv("IceCreamSales.csv");
	if(matrix==NULL)
	{
		printf("Unable to load csv file : IceCreamSales.csv\n");
		return;
	}
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%20.10lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
}


int main()
{
	test_from_csv();
	return 0;
}
