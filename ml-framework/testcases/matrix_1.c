#include<mlfw_matrix.h>
#include<mlfw_operations.h>
#include<stdio.h>
int main()
{
	int x[2][2]={{10,20},{30,40}};
	int y[2][2]={{1,2},{3,4}};
	
	mlfw_mat_double *m1,*m2,*m3;
	dimension_t rows=2;
	dimension_t columns=2;
	index_t r,c;

	m1=mlfw_mat_double_create_new(rows,columns);
	if(m1==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	m2=mlfw_mat_double_create_new(rows,columns);
	if(m2==NULL)
	{
		mlfw_mat_double_destroy(m1);
		return 0;
	}
	
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			mlfw_mat_double_set(m1,r,c,x[r][c]);
			mlfw_mat_double_set(m2,r,c,y[r][c]);
		}
	}

	m3=mlfw_multiply_double_matrix_with_matrix(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		return 0;
	}
	
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(m3,r,c));
		}
		printf("\n");
	}

	mlfw_mat_double_destroy(m1);
	mlfw_mat_double_destroy(m2);
	mlfw_mat_double_destroy(m3);
	return 0;
}
