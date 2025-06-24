#include<mlfw_matrix.h>
#include<mlfw_operations.h>
#include<stdio.h>
int main()
{
	mlfw_mat_double *m1,*m2,*m3,*m4;
	index_t r,c;

	m1=mlfw_mat_double_create_new(2,2);
	if(m1==NULL) 
	{
		printf("Low memory\n");
		return 0;
	}
	m2=mlfw_mat_double_create_new(2,2);
	if(m2==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(m1);
		return 0;
	}
	mlfw_mat_double_set(m1,0,0,1);
	mlfw_mat_double_set(m1,0,1,2);
	mlfw_mat_double_set(m1,1,0,3);
	mlfw_mat_double_set(m1,1,1,4);
	mlfw_mat_double_set(m2,0,0,10);
	mlfw_mat_double_set(m2,0,1,20);
	mlfw_mat_double_set(m2,1,0,30);
	mlfw_mat_double_set(m2,1,1,40);

	m3=mlfw_add_double_matrix_with_matrix(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		return 0;
	}
	m4=mlfw_subtract_double_matrix_from_matrix(m1,m2,NULL);
	if(m4==NULL)
	{	
		printf("Low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		mlfw_mat_double_destroy(m3);
		return 0;
	}

	printf("Added matrix\n");
	for(r=0;r<2;++r)
	{
		for(c=0;c<2;++c)
		{
			printf("%lf ",mlfw_mat_double_get(m3,r,c));
		}
		printf("\n");
	}

	printf("Subtracted matrix\n");
	for(r=0;r<2;++r)
	{
		for(c=0;c<2;++c)
		{
			printf("%lf ",mlfw_mat_double_get(m4,r,c));
		}
		printf("\n");
	}

	mlfw_mat_double_destroy(m1);
	mlfw_mat_double_destroy(m2);
	mlfw_mat_double_destroy(m3);
	mlfw_mat_double_destroy(m4);
}
