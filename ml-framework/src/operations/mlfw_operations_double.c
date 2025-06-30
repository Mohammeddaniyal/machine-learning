#include<stdio.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<math.h>
mlfw_column_vec_double * mlfw_multiply_double_row_vector_with_column_vector(mlfw_row_vec_double *left_vector,mlfw_column_vec_double *right_vector,mlfw_column_vec_double *product_vector)
{
	dimension_t left_vector_size;
	dimension_t right_vector_size;

	double left_vector_value;
	double right_vector_value;

	double product;
	
	index_t i;

	if(left_vector==NULL || right_vector==NULL) return NULL;
	
	left_vector_size=mlfw_row_vec_double_get_size(left_vector);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	if(left_vector_size!=right_vector_size) return NULL;

	if(product_vector==NULL)
	{
	// create column vector to store result of row_vector x column_vector 
	// 100% result will be a column vector of 1 row
	product_vector=mlfw_column_vec_double_create_new(1);
	if(product_vector==NULL) return NULL;
	}
	else
	{
		if(mlfw_column_vec_double_get_size(product_vector)!=1) return NULL;
	}
	product=0.0;
	for(i=0;i<left_vector_size;++i)
	{
		left_vector_value=mlfw_row_vec_double_get(left_vector,i);
		right_vector_value=mlfw_column_vec_double_get(right_vector,i);
		product=product+(left_vector_value*right_vector_value);
	}
	mlfw_column_vec_double_set(product_vector,0,product);
	return product_vector;
}
mlfw_column_vec_double * mlfw_multiply_double_matrix_with_column_vector(mlfw_mat_double *left_matrix,mlfw_column_vec_double * right_vector,mlfw_column_vec_double *product_vector)
{

	dimension_t left_matrix_rows;
	dimension_t left_matrix_columns;
	
	dimension_t right_vector_size;
	
	index_t r,c;
	
	double left_matrix_value;
	double right_vector_value;
	
	double product;
	
	if(left_matrix==NULL || right_vector==NULL) return NULL;
	
	mlfw_mat_double_get_dimensions(left_matrix,&left_matrix_rows,&left_matrix_columns);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);

	
	if(left_matrix_columns!=right_vector_size) return NULL;
	
	if(product_vector==NULL)
	{
	product_vector=mlfw_column_vec_double_create_new(left_matrix_rows);
	if(product_vector==NULL) return NULL;
	}
	else
	{
		if(mlfw_column_vec_double_get_size(product_vector)!=left_matrix_rows) return NULL;
	}
	for(r=0;r<left_matrix_rows;++r)
	{
		product=0.0;
		for(c=0;c<left_matrix_columns;++c)
		{
			left_matrix_value=mlfw_mat_double_get(left_matrix,r,c);
			right_vector_value=mlfw_column_vec_double_get(right_vector,c);
			product=product+(left_matrix_value*right_vector_value);
		}
		mlfw_column_vec_double_set(product_vector,r,product);
	}
	return product_vector;
}
mlfw_column_vec_double * mlfw_subtract_double_column_vector(mlfw_column_vec_double *left_vector,mlfw_column_vec_double *right_vector,mlfw_column_vec_double *difference_vector)
{	
	dimension_t left_vector_size;
	dimension_t right_vector_size;

	double left_vector_value;
	double right_vector_value;

	double difference;
	
	index_t i;
	if(left_vector==NULL || right_vector==NULL) return NULL;
	
	left_vector_size=mlfw_column_vec_double_get_size(left_vector);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	if(left_vector_size!=right_vector_size) return NULL;

	if(difference_vector==NULL)
	{
	difference_vector=mlfw_column_vec_double_create_new(left_vector_size);
	if(difference_vector==NULL) return NULL;
	}
	else
	{
	if(mlfw_column_vec_double_get_size(difference_vector)!=left_vector_size) return NULL;
	}
	for(i=0;i<left_vector_size;++i)
	{
		left_vector_value=mlfw_column_vec_double_get(left_vector,i);
		right_vector_value=mlfw_column_vec_double_get(right_vector,i);
		difference=left_vector_value-right_vector_value;
		mlfw_column_vec_double_set(difference_vector,i,difference);
	}
	return difference_vector;

}

mlfw_column_vec_double * mlfw_multiply_double_scalar_with_column_vector(double scalar_value,mlfw_column_vec_double *vector,mlfw_column_vec_double *product_vector)
{
	index_t i;
	dimension_t vector_size;
	double value;
	double product;
	if(vector==NULL) return NULL;
	vector_size=mlfw_column_vec_double_get_size(vector);
	if(product_vector==NULL)
	{
	product_vector=mlfw_column_vec_double_create_new(vector_size);
	if(product_vector==NULL) return NULL;
	}
	else
	{
	if(mlfw_column_vec_double_get_size(product_vector)!=vector_size) return NULL;
	}
	for(i=0;i<vector_size;++i)
	{
		value=mlfw_column_vec_double_get(vector,i);
		product=scalar_value*value;
		mlfw_column_vec_double_set(product_vector,i,product);
	}
	return product_vector;
}



mlfw_mat_double * mlfw_multiply_double_matrix_with_matrix(mlfw_mat_double *left_matrix,mlfw_mat_double *right_matrix,mlfw_mat_double *product_matrix)
{
	dimension_t m_1_rows,m_1_columns;
	dimension_t m_2_rows,m_2_columns;
	dimension_t m_3_rows,m_3_columns;
	index_t c1;
	index_t r3,c3;
	double value_1;
	double value_2;
	double value_3;
	mlfw_mat_double *m1,*m2,*m3;
	if(left_matrix==NULL || right_matrix==NULL) return NULL;
	m1=left_matrix;
	m2=right_matrix;
	mlfw_mat_double_get_dimensions(m1,&m_1_rows,&m_1_columns);
	mlfw_mat_double_get_dimensions(m2,&m_2_rows,&m_2_columns);
	if(m_1_columns!=m_2_rows) return NULL;

	if(product_matrix==NULL)
	{
		m_3_rows=m_1_rows;
		m_3_columns=m_2_columns;
		m3=mlfw_mat_double_create_new(m_3_rows,m_3_columns);
		if(m3==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(product_matrix,&m_3_rows,&m_3_columns);
		if(m_3_rows!=m_1_rows || m_3_columns!=m_2_columns) return NULL;
		m3=product_matrix;
	}

	// for logic checkout M3 L5 from begin of the lecture
 	for(r3=0;r3<m_3_rows;++r3)
	{
		for(c3=0;c3<m_3_columns;++c3)
		{
			value_3=0.0;
			for(c1=0;c1<m_1_columns;++c1)
			{
				value_1=mlfw_mat_double_get(m1,r3,c1);
				value_2=mlfw_mat_double_get(m2,c1,c3);
				value_3=value_3+(value_1*value_2);
			}
			mlfw_mat_double_set(m3,r3,c3,value_3);
		}
	}

	return m3;
}


mlfw_mat_double * mlfw_add_double_matrix_with_matrix(mlfw_mat_double *left_matrix,mlfw_mat_double *right_matrix,mlfw_mat_double *new_matrix)
{
	dimension_t left_matrix_rows,left_matrix_columns;
	dimension_t right_matrix_rows,right_matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;

	mlfw_mat_double *result;
	double left_value,right_value,result_value;
	index_t r,c;
	if(left_matrix==NULL || right_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(left_matrix,&left_matrix_rows,&left_matrix_columns);
	mlfw_mat_double_get_dimensions(right_matrix,&right_matrix_rows,&right_matrix_columns);
	if(left_matrix_rows!=right_matrix_rows || left_matrix_columns!=right_matrix_columns) 
	{
		return NULL;
	}
	if(new_matrix==NULL)
	{
		new_matrix=mlfw_mat_double_create_new(left_matrix_rows,left_matrix_columns);
		if(new_matrix==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(left_matrix_rows!=new_matrix_rows || left_matrix_columns!=new_matrix_columns)
		{
			return NULL;
		}
	}
	result=new_matrix;

	for(r=0;r<left_matrix_rows;++r)
	{
		for(c=0;c<left_matrix_columns;++c)
		{
			left_value=mlfw_mat_double_get(left_matrix,r,c);
			right_value=mlfw_mat_double_get(right_matrix,r,c);
			result_value=left_value+right_value;
			mlfw_mat_double_set(result,r,c,result_value);
		}
	}

	return result;
}

mlfw_mat_double * mlfw_subtract_double_matrix_from_matrix(mlfw_mat_double *left_matrix,mlfw_mat_double *right_matrix,mlfw_mat_double *new_matrix)
{
	dimension_t left_matrix_rows,left_matrix_columns;
	dimension_t right_matrix_rows,right_matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;

	mlfw_mat_double *result;
	double left_value,right_value,result_value;
	index_t r,c;
	if(left_matrix==NULL || right_matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(left_matrix,&left_matrix_rows,&left_matrix_columns);
	mlfw_mat_double_get_dimensions(right_matrix,&right_matrix_rows,&right_matrix_columns);
	if(left_matrix_rows!=right_matrix_rows || left_matrix_columns!=right_matrix_columns) 
	{
		return NULL;
	}
	if(new_matrix==NULL)
	{
		new_matrix=mlfw_mat_double_create_new(left_matrix_rows,left_matrix_columns);
		if(new_matrix==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(left_matrix_rows!=new_matrix_rows || left_matrix_columns!=new_matrix_columns)
		{
			return NULL;
		}
	}
	result=new_matrix;

	for(r=0;r<left_matrix_rows;++r)
	{
		for(c=0;c<left_matrix_columns;++c)
		{
			left_value=mlfw_mat_double_get(left_matrix,r,c);
			right_value=mlfw_mat_double_get(right_matrix,r,c);
			result_value=left_value-right_value;
			mlfw_mat_double_set(result,r,c,result_value);
		}
	}

	return result;
}
// it is private function, prototype not declared in header file
mlfw_mat_double * _mlfw_multiply_double_matrix_with_matrix_strassens(mlfw_mat_double *m1,mlfw_mat_double *m2,mlfw_mat_double *new_matrix)
{
	mlfw_mat_double *product=NULL;
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
		if(F==NULL)
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
			return NULL;
		}
		G=mlfw_mat_double_create_new(half,half);
		if(G==NULL)
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
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			return NULL;
		}

		E1=_mlfw_multiply_double_matrix_with_matrix_strassens(A,t1,NULL);
		if(E1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		// (A+B)*H
		t1=mlfw_add_double_matrix_with_matrix(A,B,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			return NULL;
		}

		E2=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,H,NULL);
		if(E2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		// (C+D)*E
		t1=mlfw_add_double_matrix_with_matrix(C,D,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			return NULL;
		}

		E3=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,E,NULL);
		if(E3==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);

		// D*(G-E)
		t1=mlfw_subtract_double_matrix_from_matrix(G,E,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			return NULL;
		}

		E4=_mlfw_multiply_double_matrix_with_matrix_strassens(D,t1,NULL);
		if(E4==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		// (A+D)*(E+H)
		t1=mlfw_add_double_matrix_with_matrix(A,D,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			return NULL;
		}

		t2=mlfw_add_double_matrix_with_matrix(E,H,NULL);
		if(t2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		E5=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2,NULL);
		if(E5==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(t1);
			mlfw_mat_double_destroy(t2);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// (B-D)*(G+H)
		t1=mlfw_subtract_double_matrix_from_matrix(B,D,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			return NULL;
		}

		t2=mlfw_add_double_matrix_with_matrix(G,H,NULL);
		if(t2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		E6=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2,NULL);
		if(E6==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(t1);
			mlfw_mat_double_destroy(t2);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// (A-C)*(E+F)
		t1=mlfw_subtract_double_matrix_from_matrix(A,C,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			return NULL;
		}

		t2=mlfw_add_double_matrix_with_matrix(E,F,NULL);
		if(t2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		E7=_mlfw_multiply_double_matrix_with_matrix_strassens(t1,t2,NULL);
		if(E7==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(t1);
			mlfw_mat_double_destroy(t2);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);

		// CELL_0_0=E5+E4-E2+E6
		t1=mlfw_add_double_matrix_with_matrix(E5,E4,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			return NULL;
		}

		t2=mlfw_subtract_double_matrix_from_matrix(t1,E2,NULL);
		if(t2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		CELL_0_0=mlfw_add_double_matrix_with_matrix(t2,E6,NULL);
		if(CELL_0_0==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			mlfw_mat_double_destroy(t1);
			mlfw_mat_double_destroy(t2);
			return NULL;
		}

		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);
		// CELL_0_1=E1+E2
		CELL_0_1=mlfw_add_double_matrix_with_matrix(E1,E2,NULL);
		if(CELL_0_1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(CELL_0_0);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			return NULL;
		}

		// CELL_1_0=E3+E4
		CELL_1_0=mlfw_add_double_matrix_with_matrix(E3,E4,NULL);
		if(CELL_1_0==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(CELL_0_0);
			mlfw_mat_double_destroy(CELL_0_1);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			return NULL;
		}

		// CELL_1_1=E1+E5-E3-E7
		t1=mlfw_add_double_matrix_with_matrix(E1,E5,NULL);
		if(t1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(CELL_0_0);
			mlfw_mat_double_destroy(CELL_0_1);
			mlfw_mat_double_destroy(CELL_1_0);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			return NULL;
		}

		t2=mlfw_subtract_double_matrix_from_matrix(t1,E3,NULL);
		if(t2==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(CELL_0_0);
			mlfw_mat_double_destroy(CELL_0_1);
			mlfw_mat_double_destroy(CELL_1_0);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			mlfw_mat_double_destroy(t1);
			return NULL;
		}

		CELL_1_1=mlfw_subtract_double_matrix_from_matrix(t2,E7,NULL);
		if(CELL_1_1==NULL)
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
			mlfw_mat_double_destroy(H);
			mlfw_mat_double_destroy(CELL_0_0);
			mlfw_mat_double_destroy(CELL_0_1);
			mlfw_mat_double_destroy(CELL_1_0);
			mlfw_mat_double_destroy(E1);
			mlfw_mat_double_destroy(E2);
			mlfw_mat_double_destroy(E3);
			mlfw_mat_double_destroy(E4);
			mlfw_mat_double_destroy(E5);
			mlfw_mat_double_destroy(E6);
			mlfw_mat_double_destroy(E7);
			mlfw_mat_double_destroy(t1);
			mlfw_mat_double_destroy(t2);
			return NULL;
		}


		mlfw_mat_double_destroy(t1);
		mlfw_mat_double_destroy(t2);

		mlfw_mat_double_copy(product,CELL_0_0,0,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_0_1,0,half,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_1_0,half,0,0,0,half-1,half-1);
		mlfw_mat_double_copy(product,CELL_1_1,half,half,0,0,half-1,half-1);

		mlfw_mat_double_destroy(A);
		mlfw_mat_double_destroy(B);
		mlfw_mat_double_destroy(C);
		mlfw_mat_double_destroy(D);
		mlfw_mat_double_destroy(E);
		mlfw_mat_double_destroy(F);
		mlfw_mat_double_destroy(G);
		mlfw_mat_double_destroy(H);
	
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
	double y,n;
	dimension_t m1_rows,m1_columns;
	dimension_t m2_rows,m2_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	if(m1==NULL || m2==NULL) return NULL;
	mlfw_mat_double_get_dimensions(m1,&m1_rows,&m1_columns);
	if(m1_rows!=m1_columns) return NULL;

	// checking dimensions are in the power of 2
	y=(double)m1_rows;
	n=log2(y);
	if(floor(n)!=ceil(n)) return NULL;

	mlfw_mat_double_get_dimensions(m2,&m2_rows,&m2_columns);
	if(m2_rows!=m2_columns || m2_rows!=m1_rows) return NULL;
	
	if(new_matrix!=NULL)
	{
		mlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(new_matrix_rows!=new_matrix_columns || new_matrix_rows!=m1_rows) return NULL;
	}

	product=_mlfw_multiply_double_matrix_with_matrix_strassens(m1,m2,new_matrix);
	return product;
}


mlfw_column_vec_double * mlfw_subtract_double_column_vector_from_scalar(mlfw_column_vec_double *vector,double scalar_value,mlfw_column_vec_double *new_vector)
{
	mlfw_column_vec_double *v;
	dimension_t vector_size;
	dimension_t new_vector_size;
	double result;
	double value;
	index_t i;
	if(vector==NULL) return NULL;
	vector_size=mlfw_column_vec_double_get_size(vector);
	if(new_vector==NULL)
	{
		v=mlfw_column_vec_double_create_new(vector_size);
		if(v==NULL) return NULL;
	}
	else
	{
		new_vector_size=mlfw_column_vec_double_get_size(new_vector);
		if(new_vector_size!=vector_size) return NULL;
		v=new_vector;
	}
	for(i=0;i<vector_size;++i)
	{
		value=mlfw_column_vec_double_get(vector,i);
		result=scalar_value-value;
		mlfw_column_vec_double_set(v,i,result);
	}
	return v;
}


mlfw_column_vec_double * mlfw_element_wise_multiply_double_column_vector(mlfw_column_vec_double *left_vector,mlfw_column_vec_double *right_vector,mlfw_column_vec_double *new_vector)
{
	mlfw_column_vec_double *v;
	dimension_t left_vector_size;
	dimension_t right_vector_size;
	dimension_t new_vector_size;
	double value1;
	double value2;
	double result;
	index_t;
	if(left_vector==NULL || right_vector==NULL) return NULL;
	left_vector_size=mlfw_column_vec_double_get_size(left_vector);
	right_vector_size=mlfw_column_vec_double_get_size(right_vector);
	if(left_vector_size!=right_vector_size) return NULL;
	if(new_matrix==NULL)
	{
		v=mlfw_column_vec_double_create_new(left_vector_size);
		if(v==NULL) return NULL;
	}
	else
	{
		new_vector_size=mlfw_column_vec_double_get_size(new_vector);
		if(left_vector_size!=new_vector_size) return NULL;
		v=new_vector;
	}
	for(i=0;i<left_vector_size;++i)
	{
		value1=mlfw_column_vec_double_get(left_vector,i);
		value2=mlfw_column_vec_double_get(right_vector,i);
		result=value1*value2;
		mlfw_column_vec_double_set(v,i,result);
	}
	return v;
}
