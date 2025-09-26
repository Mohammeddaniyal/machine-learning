#include<dmlfw_matrix.h>
#include<dmlfw_operations.h>
#include<stdio.h>
int main()
{
	int x[2][2]={{10,20},{30,40}};
	int y[2][2]={{1,2},{3,4}};
	
	dmlfw_mat_double *m1,*m2,*m3;
	dimension_t rows=2;
	dimension_t columns=2;
	index_t r,c;

	m1=dmlfw_mat_double_create_new(rows,columns);
	if(m1==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	m2=dmlfw_mat_double_create_new(rows,columns);
	if(m2==NULL)
	{
		dmlfw_mat_double_destroy(m1);
		return 0;
	}
	
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			dmlfw_mat_double_set(m1,r,c,x[r][c]);
			dmlfw_mat_double_set(m2,r,c,y[r][c]);
		}
	}

	m3=dmlfw_multiply_double_matrix_with_matrix(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(m1);
		dmlfw_mat_double_destroy(m2);
		return 0;
	}
	
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(m3,r,c));
		}
		printf("\n");
	}

	dmlfw_mat_double_destroy(m1);
	dmlfw_mat_double_destroy(m2);
	dmlfw_mat_double_destroy(m3);
	return 0;
}
