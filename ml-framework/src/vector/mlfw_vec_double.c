#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>
typedef struct __mlfw_column_vec_double
{
	double *data;
	dimension_t size;
}mlfw_column_vec_double;
typedef struct __mlfw_row_vec_double
{
	double *data;
	dimension_t size;
}mlfw_row_vec_double;

// column vector function implementations

mlfw_column_vec_double * mlfw_column_vec_double_create_new(dimension_t size)
{
	mlfw_column_vec_double *vector;
	if(size<=0) return NULL;
	vector=(mlfw_column_vec_double *)malloc(sizeof(mlfw_column_vec_double));
	if(vector==NULL) return NULL;
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		free(vector);
		return NULL;
	}
	vector->size=size;
	return vector;
}
void mlfw_column_vec_double_destroy(mlfw_column_vec_double *vector)
{
	if(vector==NULL) return;
	free(vector->data);
	free(vector);
}
double mlfw_column_vec_double_get(mlfw_column_vec_double *vector,index_t index)
{
	if(vector==NULL) return 0.0;
	if(index<0 || index>=vector->size) return 0.0;
	return vector->data[index];
}
void mlfw_column_vec_double_set(mlfw_column_vec_double *vector,index_t index,double value)
{
	if(vector==NULL) return;
	if(index<0 || index>=vector->size) return;
	vector->data[index]=value;
}
mlfw_column_vec_double * mlfw_column_vec_double_create_new_filled(dimension_t size,double value)
{
	index_t i;
	mlfw_column_vec_double *vector;
	if(size<=0) return NULL;
	vector=mlfw_column_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	for(i=0;i<vector->size;++i) vector->data[i]=value;
	return vector;
}
mlfw_row_vec_double * mlfw_column_vec_double_transpose(mlfw_column_vec_double *vector)
{
	index_t i;
	mlfw_row_vec_double *transposed_vector;
	if(vector==NULL) return NULL;
	transposed_vector=mlfw_row_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;

}
dimension_t mlfw_column_vec_double_get_size(mlfw_column_vec_double *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}



double mlfw_column_vec_double_get_mean(mlfw_column_vec_double *vector)
{
	index_t i;
	double sum;
	double mean;
	if(vector==NULL) return 0.0;
	sum=0.0;
	for(i=0;i<vector->size;++i)
	{
		sum=sum+vector->data[i];
	}
	mean=sum/(double)vector->size;
	return mean;
}

// row vector function implementations

mlfw_row_vec_double * mlfw_row_vec_double_create_new(dimension_t size)
{
	mlfw_row_vec_double *vector;
	if(size<=0) return NULL;
	vector=(mlfw_row_vec_double *)malloc(sizeof(mlfw_row_vec_double));
	if(vector==NULL) return NULL;
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		free(vector);
		return NULL;
	}
	vector->size=size;
	return vector;
}
void mlfw_row_vec_double_destroy(mlfw_row_vec_double *vector)
{
	if(vector==NULL) return;
	free(vector->data);
	free(vector);
}
double mlfw_row_vec_double_get(mlfw_row_vec_double *vector,index_t index)
{
	if(vector==NULL) return 0.0;
	if(index<0 || index>=vector->size) return 0.0;
	return vector->data[index];
}
void mlfw_row_vec_double_set(mlfw_row_vec_double *vector,index_t index,double value)
{
	if(vector==NULL) return;
	if(index<0 || index>=vector->size) return;
	vector->data[index]=value;
}
mlfw_row_vec_double * mlfw_row_vec_double_create_new_filled(dimension_t size,double value)
{
	index_t i;
	mlfw_row_vec_double *vector;
	if(size<=0) return NULL;
	vector=mlfw_row_vec_double_create_new(size);
	if(vector==NULL) return NULL;
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_column_vec_double * mlfw_row_vec_double_transpose(mlfw_row_vec_double *vector)
{
	index_t i;
	mlfw_column_vec_double *transposed_vector;
	if(vector==NULL) return NULL;
	transposed_vector=mlfw_column_vec_double_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;
}
dimension_t mlfw_row_vec_double_get_size(mlfw_row_vec_double *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}


double mlfw_row_vec_double_get_mean(mlfw_row_vec_double *vector)
{
	index_t i;
	double mean,sum;
	if(vector==NULL) return 0.0;
	for(i=0,sum=0.0;i<vector->size;++i) sum+=vector->data[i];
	mean=sum/(double)vector->size;
	return mean;
}
