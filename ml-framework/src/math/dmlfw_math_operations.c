#include<dmlfw_math.h>
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<stdio.h>
#include<math.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

dmlfw_column_vec_double * dmlfw_column_vec_double_sigmoid(dmlfw_column_vec_double *vector,dmlfw_column_vec_double *new_vector)
{
	dmlfw_column_vec_double *v;
	dimension_t vector_size;
	dimension_t new_vector_size;
	double value;
	double sigmoid;
	double e;
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "vector");
		return NULL;
	}
	vector_size=dmlfw_column_vec_double_get_size(vector);
	if(new_vector==NULL)
	{
		v=dmlfw_column_vec_double_create_new(vector_size);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		new_vector_size=dmlfw_column_vec_double_get_size(new_vector);
		if(new_vector_size!=vector_size)
		{
		   	_dmlfw_set_error(MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT_CODE,MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT,"new_vector", new_vector_size, vector_size);
			return NULL;
		}
		v=new_vector;
	}
		// formula is 1/(1+e^-z)
	e=2.718281; // napier's constant
	for(i=0;i<vector_size;++i)
	{
		value=dmlfw_column_vec_double_get(vector,i);
		sigmoid=1/(1+pow(e,-value));
		dmlfw_column_vec_double_set(v,i,sigmoid);
	}
	return v;
}

dmlfw_column_vec_double * dmlfw_column_vec_double_log(dmlfw_column_vec_double *vector,dmlfw_column_vec_double *new_vector)
{
	dmlfw_column_vec_double *v;
	dimension_t vector_size;
	dimension_t new_vector_size;
	double value;
	double log_value;
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL)
	{        
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "vector");
		return NULL;
	}
	vector_size=dmlfw_column_vec_double_get_size(vector);
	if(new_vector==NULL)
	{
		v=dmlfw_column_vec_double_create_new(vector_size);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		new_vector_size=dmlfw_column_vec_double_get_size(new_vector);
		if(new_vector_size!=vector_size) 
		{
		   	_dmlfw_set_error(MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT_CODE,MLFW_INVALID_VECTOR_CONTAINER_SIZE_TO_STORE_RESULT,"new_vector", new_vector_size, vector_size);
			return NULL;
		}
		v=new_vector;
	}
	for(i=0;i<vector_size;++i)
	{
		value=dmlfw_column_vec_double_get(vector,i);
		log_value=log(value);
		dmlfw_column_vec_double_set(v,i,log_value);
	}
	return v;

}

double dmlfw_column_vec_double_sum(dmlfw_column_vec_double *vector)
{
	dimension_t vector_size;
	double value;
	double sum;
	index_t i;
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "vector");
		return 0.0;
	}
	vector_size=dmlfw_column_vec_double_get_size(vector);
	sum=0;
	for(i=0;i<vector_size;++i)
	{
		value=dmlfw_column_vec_double_get(vector,i);
		sum=sum+value;
	}
	return sum;


}

dmlfw_mat_double * dmlfw_mat_double_sigmoid(dmlfw_mat_double *matrix,dmlfw_mat_double *new_matrix)
{
	dmlfw_mat_double *m;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;;
	double value;
	double sigmoid;
	double e;
	index_t r,c;
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
	 	return NULL;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(new_matrix==NULL)
	{
		m=dmlfw_mat_double_create_new(matrix_rows,matrix_columns);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(new_matrix_rows!=matrix_rows || new_matrix_columns!=matrix_columns) 
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT, "new_matrix",new_matrix_rows, new_matrix_columns, matrix_rows, matrix_columns);
			return NULL;
		}
		m=new_matrix;
	}
		// formula is 1/(1+e^-z)
	e=2.718281; // napier's constant
	for(r=0;r<matrix_rows;++r)
	{
	for(c=0;c<matrix_columns;++c)
	{
		value=dmlfw_mat_double_get(matrix,r,c);
		sigmoid=1/(1+pow(e,-value));
		dmlfw_mat_double_set(m,r,c,sigmoid);
	}
	}
	return m;
}
