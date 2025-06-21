#include<stdio.h>
#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>

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



mlfw_mat_double * mlfw_mat_double_multiply_matrix_with_matrix(mlfw_mat_double *left_matrix,mlfw_mat_double *right_matrix,mlfw_mat_double *product_matrix)
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
				if(value_1==0 || value_2==0) continue;
				value_3=value_3+(value_1*value_2);
			}
			mlfw_mat_double_set(m3,r3,c3,value_3);
		}
	}

	return m3;
}
