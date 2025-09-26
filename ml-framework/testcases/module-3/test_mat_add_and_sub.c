#include<dmlfw_matrix.h>
#include<dmlfw_operations.h>
#include<stdio.h>
int main()
{
	dmlfw_mat_double *m1,*m2,*m3,*m4;
	index_t r,c;

	m1=dmlfw_mat_double_create_new(2,2);
	if(m1==NULL) 
	{
		printf("Low memory\n");
		return 0;
	}
	m2=dmlfw_mat_double_create_new(2,2);
	if(m2==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(m1);
		return 0;
	}
	dmlfw_mat_double_set(m1,0,0,1);
	dmlfw_mat_double_set(m1,0,1,2);
	dmlfw_mat_double_set(m1,1,0,3);
	dmlfw_mat_double_set(m1,1,1,4);
	dmlfw_mat_double_set(m2,0,0,10);
	dmlfw_mat_double_set(m2,0,1,20);
	dmlfw_mat_double_set(m2,1,0,30);
	dmlfw_mat_double_set(m2,1,1,40);

	m3=dmlfw_add_double_matrix_with_matrix(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(m1);
		dmlfw_mat_double_destroy(m2);
		return 0;
	}
	m4=dmlfw_subtract_double_matrix_from_matrix(m1,m2,NULL);
	if(m4==NULL)
	{	
		printf("Low memory\n");
		dmlfw_mat_double_destroy(m1);
		dmlfw_mat_double_destroy(m2);
		dmlfw_mat_double_destroy(m3);
		return 0;
	}

	printf("Added matrix\n");
	for(r=0;r<2;++r)
	{
		for(c=0;c<2;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(m3,r,c));
		}
		printf("\n");
	}

	printf("Subtracted matrix\n");
	for(r=0;r<2;++r)
	{
		for(c=0;c<2;++c)
		{
			printf("%lf ",dmlfw_mat_double_get(m4,r,c));
		}
		printf("\n");
	}

	dmlfw_mat_double_destroy(m1);
	dmlfw_mat_double_destroy(m2);
	dmlfw_mat_double_destroy(m3);
	dmlfw_mat_double_destroy(m4);
}
