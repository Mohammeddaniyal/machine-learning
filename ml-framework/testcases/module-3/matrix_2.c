#include<mlfw_matrix.h>
#include<stdio.h>
int main()
{
	int x[2][2]={{10,20},{30,40}};
	int y[2][2]={{1,2},{3,4}};
	
	mlfw_mat_double *m1,*m2,*m3;
	dimension_t rows=2;
	dimension_t columns=2;
	index_t r,c;
	
	double A,B,C,D,E,F,G,H;
	double e1,e2,e3,e4,e5,e6,e7;

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

	m3=mlfw_mat_double_create_new(rows,columns);
	if(m3==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		return 0;
	}
	
	A=mlfw_mat_double_get(m1,0,0);
	B=mlfw_mat_double_get(m1,0,1);
	C=mlfw_mat_double_get(m1,1,0);
	D=mlfw_mat_double_get(m1,1,1);
	E=mlfw_mat_double_get(m2,0,0);
	F=mlfw_mat_double_get(m2,0,1);
	G=mlfw_mat_double_get(m2,1,0);
	H=mlfw_mat_double_get(m2,1,1);

	e1=A*(F-H);
	e2=(A+B)*H;
	e3=(C+D)*E;
	e4=D*(G-E);
	e5=(A+D)*(E+H);
	e6=(B-D)*(G+H);
	e7=(A-C)*(E+F);

	mlfw_mat_double_set(m3,0,0,e5+e4-e2+e6);
	mlfw_mat_double_set(m3,0,1,e1+e2);
	mlfw_mat_double_set(m3,1,0,e3+e4);
	mlfw_mat_double_set(m3,1,1,e1+e5-e3-e7);


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
