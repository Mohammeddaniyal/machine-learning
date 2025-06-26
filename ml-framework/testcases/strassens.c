#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>

mlfw_mat_double * _mlfw_multiply_double_matrix_with_matrix_strassens(mlfw_mat_double *m1,mlfw_mat_double *m2,mlfw_mat_double *new_matrix)
{
	mlfw_mat_double *product;
	dimension_t rows,columns;
	dimension_t n,half;
	
	double a,b,c,d,e,f,g,h;
	double e1,e2,e3,e4,e5,e6,e7;
	double cell_0_0,cell_0_1,cell_1_0,cell_1_1;


	mlfw_mat_double *A,*B,*C,*D,*E,*F,*G,*H;
	mlfw_mat_double *E1,*E2,*E3,*E4,*E5,*E6,*E7;
	mlfw_mat_double *CELL_0_0,*CELL_0_1,*CELL_1_0,*CELL_1_1;
	mlfw_mat_double *t1,*t2;

	mlfw_mat_double_get_dimensions(m1,&rows,&columns);
	n=rows;
	if(new_matrix==NULL)
	{
		product=mlfw_mat_double_create_new(n,n);
	}
	else
	{
		product=new_matrix;
	}
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
		
		e1=a*(f-h);	
		e2=(a+b)*h;
		e3=(c+d)*e;
		e4=d*(g-e);
		e5=(a+d)*(e+h);
		e6=(b-d)*(g+h);
		e7=(a-c)*(e+f);
		
		cell_0_0=e5+e4-e2+e6;
		cell_0_1=e1+e2;
		cell_1_0=e3+e4;
		cell_1_1=e1+e5-e3-e7;

		mlfw_mat_double_set(product,0,0,cell_0_0);
		mlfw_mat_double_set(product,0,1,cell_0_1);
		mlfw_mat_double_set(product,1,0,cell_1_0);
		mlfw_mat_double_set(product,1,1,cell_1_1);
	}
	else
	{
		half=n/2;
		A=mlfw_mat_double_create_new(half,half);
		if(A==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			return NULL;
		}
		B=mlfw_mat_double_create_new(half,half);
		if(B==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			return NULL;
		}
		C=mlfw_mat_double_create_new(half,half);
		if(C==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			return NULL;
		}
		D=mlfw_mat_double_create_new(half,half);
		if(D==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			mlfw_mat_double_destroy(C);
			return NULL;
		}
		E=mlfw_mat_double_create_new(half,half);
		if(E==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			mlfw_mat_double_destroy(C);
			mlfw_mat_double_destroy(D);
			return NULL;
		}
		F=mlfw_mat_double_create_new(half,half);
		if(H==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			mlfw_mat_double_destroy(C);
			mlfw_mat_double_destroy(D);
			mlfw_mat_double_destroy(E);
			mlfw_mat_double_destroy(F);
			mlfw_mat_double_destroy(G);
			return NULL;
		}
		G=mlfw_mat_double_create_new(half,half);
		if(H==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			mlfw_mat_double_destroy(C);
			mlfw_mat_double_destroy(D);
			mlfw_mat_double_destroy(E);
			mlfw_mat_double_destroy(F);
			mlfw_mat_double_destroy(G);
			return NULL;
		}
		H=mlfw_mat_double_create_new(half,half);
		if(H==NULL)
		{
			if(new_matrix==NULL)
			{
				mlfw_mat_double_destroy(product);
			}
			mlfw_mat_double_destroy(A);
			mlfw_mat_double_destroy(B);
			mlfw_mat_double_destroy(C);
			mlfw_mat_double_destroy(D);
			mlfw_mat_double_destroy(E);
			mlfw_mat_double_destroy(F);
			mlfw_mat_double_destroy(G);
			return NULL;
		}

		mlfw_mat_double_copy(A,m1,0,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(B,m1,0,0,0,half,half-1,n-1);
		mlfw_mat_double_copy(C,m1,0,0,half,0,n-1,half-1);
		mlfw_mat_double_copy(D,m1,0,0,half,half,n-1,n-1);
		
		mlfw_mat_double_copy(E,m2,0,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(F,m2,0,0,0,half,half-1,n-1);
		mlfw_mat_double_copy(G,m2,0,0,half,0,n-1,half-1);
		mlfw_mat_double_copy(H,m2,0,0,half,half,n-1,n-1);

		// A*(F-H)
		t1=mlfw_subtract_double_matrix_from_matrix(F,H,NULL);
		E1=_mlfw_multiply_double_matrix_with_matrix_strassens(A,t1);
		mlfw_mat_double_destroy(t1);
		// (A+B)*H
		t1=mlfw_add_double_matrix_with_matrix(A,B,NULL);
		E2=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,H);
		mlfw_mat_double_destroy(t1);
		// (C+D)*E
		t1=mlfw_add_double_matrix_with_matrix(C,D,NULL);
		E3=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,E);
		mlfw_mat_double_destroy(t1);

		// D*(G-E)
		t1=mlfw_subtract_double_matrix_from_matrix(G,E,NULL);
		E4=_mlfw_multiply_double_matrix_with_matrix_strassens(D,t1);
		mlfw_mat_double_destroy(t1);
		// (A+D)*(E+H)
		t1=mlfw_add_double_matrix_with_matrix(A,D,NULL);
		t2=mlfw_add_double_matrix_with_matrix(E,H,NULL);
		E5=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2);
		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// (B-D)*(G+H)
		t1=mlfw_subtract_double_matrix_from_matrix(B,D,NULL);
		t2=mlfw_add_double_matrix_with_matrix(G,H,NULL);
		E6=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2);
		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// (A-C)*(E+F)
		t1=mlfw_subtract_double_matrix_from_matrix(A,C,NULL);
		t2=mlfw_add_double_matrix_with_matrix(E,F,NULL);
		E7=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2);
		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);

		// CELL_0_0=E5+E4-E2+E6
		t1=mlfw_add_double_matrix_with_matrix(E5,E4,NULL);
		t2=mlfw_subtract_double_matrix_from_matrix(t1,E2,NULL);
		CELL_0_0=mlfw_add_double_matrix_with_matrix(t2,E6,NULL);
		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// CELL_0_1=E1+E2
		CELL_0_1=mlfw_add_double_matrix_with_matrix(E1,E2,NULL);
		// CELL_1_0=E3+E4
		CELL_1_0=mlfw_add_double_matrix_with_matrix(E3,E4,NULL);
		// CELL_1_1=E1+E5-E3-E7
		t1=mlfw_add_double_matrix_with_matrix(E1,E5,NULL);
		t2=mlfw_subtract_double_matrix_from_matrix(t1,E3,NULL);
		CELL_1_1=mlfw_subtract_double_matrix_from_matrix(t2,E7,NULL);
		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);

		mlfw_mat_double_copy(product,CELL_0_0,0,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_0_1,0,half,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_1_0,half,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_1_1,half,half,0,0,half-1,half-1);

		mlfw_mat_double_destroy(CELL_0_0);
		mlfw_mat_double_destroy(CELL_0_1);
		mlfw_mat_double_destroy(CELL_1_0);
		mlfw_mat_double_destroy(CELL_1_1);
		mlfw_mat_double_destroy(E1);
		mlfw_mat_double_destroy(E2);
		mlfw_mat_double_destroy(E3);
		mlfw_mat_double_destroy(E4);
		mlfw_mat_double_destroy(E5);
		mlfw_mat_double_destroy(E6);
		mlfw_mat_double_destroy(E7);
	}
	return product;
}

mlfw_mat_double * mlfw_multiply_double_matrix_with_matrix_strassens(mlfw_mat_double *m1,mlfw_mat_double *m2,mlfw_mat_double *new_matrix)
{
	mlfw_mat_double *product;
	product=_mlfw_multiply_double_matrix_with_matrix_strassens(m1,m2,new_matrix);
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
