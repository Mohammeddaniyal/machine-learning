#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>
#include<mlfw_matrix.h>
int main()
{
	mlfw_row_vec_string *header;
	mlfw_mat_double *matrix;
	char *ptr;
	double value;
	index_t r,c,i;
	dimension_t size,rows,columns;
	matrix=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&header);
	if(matrix==NULL)
	{
		printf("Unable to load the dataset\n");
		return 0;
	}
	size=mlfw_row_vec_string_get_size(header);
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(i=0;i<size;++i)
	{
		mlfw_row_vec_string_get(header,i,&ptr);
		if(ptr!=NULL)
		{
			printf("%s ",ptr);
			free(ptr);
		}
	}
	printf("\n");
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			value=mlfw_mat_double_get(matrix,r,c);
			printf("%lf ",value);		
		}
		printf("\n");
	}
	mlfw_row_vec_string_destroy(header);
	mlfw_mat_double_destroy(matrix);
	return 0;
}
