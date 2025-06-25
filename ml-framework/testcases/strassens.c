#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>

mlfw_mat_double * _mlfw_multiply_double_matrix_with_matrix_strassens(mlfw_mat_double *m1,mlfw_mat_double *m2)
{
	mlfw_mat_double *product;
	dimension_t rows,columns;
	dimension_t n;
	
	double a,b,c,d,e,f,g,h;
	double e1,e2,e3,e4,e5,e6,e7;
	double cell_0_0,cell_0_1,cell_1_0,cell_1_1;

	mlfw_mat_double_get_dimensions(m1,&rows,&columns);
	n=rows;
	product=mlfw_mat_double_create_new(n,n);
	if(n==2)
	{
		a=mlfw_mat_double_get(m1,0,0);
		b=mlfw_mat_double_get(m1,0,1);
		c=mlfw_mat_double_get(m1,1,0);
		d=mlfw_mat_double_get(m1,1,1);
	
		e=mlfw_mat_double_get(m2,0,0);
		f=mlfw_mat_double_get(m2,0,1);
		g=mlfw_mat_double_get(m2,1,0);
		h=mlfw_mat_double_get(m2,1,1);
		// a*(f-h)
		e1=a*(f-h);	
			
		// (a+b)*h
		e2=(a+b)*h;

		// (c+d)*e
		e3=(c+d)*e;

		// d*(g-e)
		e4=d*(g-e);

		// (a+d)*(e+h)
		e5=(a+d)*(e+h);

		// (b-d)*(g+h)
		e6=(b-d)*(g+h);

		// (a-c)*(e+f)
		e7=(a-c)*(e+f);

		// cell_0_0=e5+e4-e2+e6
		cell_0_0=e5+e4-e2+e6;
		// cell_0_1=e1+e2
		cell_0_1=e1+e2;
		// cell_1_0=e3+e4
		cell_1_0=e3+e4;
		// cell_1_1=e1+e5-e3-e7
		cell_1_1=e1+e5-e3-e7;

		mlfw_mat_double_set(product,0,0,cell_0_0);
		mlfw_mat_double_set(product,0,1,cell_0_1);
		mlfw_mat_double_set(product,1,0,cell_1_0);
		mlfw_mat_double_set(product,1,1,cell_1_1);
	}
	else
	{
	}
	return product;
}

mlfw_mat_double * mlfw_multiply_double_matrix_with_matrix_strassens(mlfw_mat_double *m1,mlfw_mat_double *m2,mlfw_mat_double *new_matrix)
{
	mlfw_mat_double *product;
	product=_mlfw_multiply_double_matrix_with_matrix_strassens(m1,m2);
	return product;
}

void print_matrix(mlfw_mat_double *matrix)
{
	dimension_t rows,columns;
	index_t r,c;
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
}
int main(int argc,char *argv[])
{
	mlfw_mat_double *m1,*m2,*m3;
	mlfw_row_vec_string *h1,*h2;

	if(argc!=3)
	{
		printf("Pass name of 2 csv files with matrix as command line arguments\n");
		return 0;
	}
	m1=mlfw_mat_double_from_csv(argv[1],NULL,&h1);
	if(m1==NULL)
	{
		printf("Unable to load matrix from %s\n",argv[1]);
		return 0;
	}
	m2=mlfw_mat_double_from_csv(argv[2],NULL,&h2);
	if(m2==NULL)
	{
		printf("Unable to load matrix from %s\n",argv[2]);
		mlfw_mat_double_destroy(m1);
		mlfw_row_vec_string_destroy(h1);
		return 0;
	}
	m3=mlfw_multiply_double_matrix_with_matrix_strassens(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		mlfw_row_vec_string_destroy(h1);
		mlfw_row_vec_string_destroy(h2);
		return 0;
	}
	print_matrix(m3);
	mlfw_mat_double_destroy(m1);
	mlfw_mat_double_destroy(m2);
	mlfw_mat_double_destroy(m3);
	mlfw_row_vec_string_destroy(h1);
	mlfw_row_vec_string_destroy(h2);
	return 0;
}
