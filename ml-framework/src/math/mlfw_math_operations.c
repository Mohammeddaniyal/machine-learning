#include<mlfw_math.h>
#include<mlfw_vector.h>
#include<stdio.h>
#include<math.h>
mlfw_column_vec_double * mlfw_column_vec_double_sigmoid(mlfw_column_vec_double *vector,mlfw_column_vec_double *new_vector)
{
	mlfw_column_vec_double *v;
	dimension_t vector_size;
	dimension_t new_vector_size;
	double value;
	double sigmoid;
	double e;
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
		// formula is 1/(1+e^-z)
	e=2.718281; // napier's constant
	for(i=0;i<vector_size;++i)
	{
		value=mlfw_column_vec_double_get(vector,i);
		sigmoid=1/(1+pow(e,-value));
		mlfw_column_vec_double_set(v,i,sigmoid);
	}
	return v;
}

mlfw_column_vec_double * mlfw_column_vec_double_log(mlfw_column_vec_double *vector,mlfw_column_vec_double *new_vector)
{
	mlfw_column_vec_double *v;
	dimension_t vector_size;
	dimension_t new_vector_size;
	double value;
	double log_value;
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
		log_value=log(value);
		mlfw_column_vec_double_set(v,i,log_value);
	}
	return v;

}

double mlfw_column_vec_double_sum(mlfw_column_vec_double *vector)
{
	dimension_t vector_size;
	double value;
	double sum;
	index_t i;
	if(vector==NULL) return 0.0;
	vector_size=mlfw_column_vec_double_get_size(vector);
	sum=0;
	for(i=0;i<vector_size;++i)
	{
		value=mlfw_column_vec_double_get(vector,i);
		sum=sum+value;
	}
	return sum;


}

mlfw_mat_double * mlfw_mat_double_sigmoid(mlfw_mat_double *matrix,mlfw_mat_double *new_matrix)
{
	mlfw_mat_double *m;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;;
	double value;
	double sigmoid;
	double e;
	index_t r,c;
	if(matrix==NULL) return NULL;
	mlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(new_matrix==NULL)
	{
		m=mlfw_mat_double_create_new(matrix_rows,matrix_columns);
		if(m==NULL) return NULL;
	}
	else
	{
		mlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(new_matrix_rows!=matrix_rows || new_matrix_columns!=matrix_columns) return NULL;
		m=new_matrix;
	}
		// formula is 1/(1+e^-z)
	e=2.718281; // napier's constant
	for(r=0;r<matrix_rows;++r)
	{
	for(c=0;i<matrix_columns;++c)
	{
		value=mlfw_mat_double_get(matrix,r,c);
		sigmoid=1/(1+pow(e,-value));
		mlfw_mat_double_set(matrix,r,c,sigmoid);
	}
	}
	return m;

}
