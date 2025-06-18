#include<mlfw_matrix.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *identity_matrix;
	index_t r,c;
	dimension_t rows,columns;
	identity_matrix=mlfw_mat_double_create_identity_matrix(5);
	if(identity_matrix==NULL)
	{
		printf("Unable to create identity matrix\n");
		return 0;
	}
	mlfw_mat_double_get_dimensions(identity_matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(identity_matrix,r,c));
		}
		printf("\n");
	}
	mlfw_mat_double_destroy(identity_matrix);
	return 0;
}
