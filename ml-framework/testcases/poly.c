#include<stdio.h>
#include<dmlfw_vector.h>
#include<dmlfw_list.h>


void __dmlfw_double_generate_polynomial_dataset(dmlfw_row_vec_double *dataset,index_t i,double accumulated_value,uint8_t exponent,dmlfw_forward_list_double *polynomial_dataset)
{
	double yes_value;
	dimension_t dataset_size;
	if(exponent==0) return;
	yes_value=accumulated_value*dmlfw_row_vec_double_get(dataset,i);
	dmlfw_forward_list_double_insert(polynomial_dataset,yes_value);
	__dmlfw_double_generate_polynomial_dataset(dataset,i,yes_value,exponent-1,polynomial_dataset);
	dataset_size=dmlfw_row_vec_double_get_size(dataset);
	if(i+1<dataset_size)
	{
	__dmlfw_double_generate_polynomial_dataset(dataset,i+1,accumulated_value,exponent,polynomial_dataset);
	}
}
dmlfw_row_vec_double * dmlfw_double_generate_polynomial_dataset(dmlfw_row_vec_double *dataset,uint8_t exponent)
{
	dmlfw_forward_list_double *polynomial_dataset;
	dmlfw_row_vec_double *vector;
	dimension_t polynomial_dataset_size;
	if(dataset==NULL) return NULL;
	if(exponent<1) return NULL;
	polynomial_dataset=dmlfw_forward_list_double_create_new();
	if(polynomial_dataset==NULL) return NULL;
	__dmlfw_double_generate_polynomial_dataset(dataset,0,1.0,exponent,polynomial_dataset);
	polynomial_dataset_size=dmlfw_forward_list_double_get_size(polynomial_dataset);
	if(polynomial_dataset_size==0) return NULL;
	dmlfw_forward_list_double_insert(polynomial_dataset,1.0);
	vector=dmlfw_forward_list_double_get_row_vector(polynomial_dataset);
	return vector;
}

int main()
{
	
	dmlfw_row_vec_double *dataset;
	dmlfw_row_vec_double *polynomial_dataset;
	dimension_t polynomial_dataset_size;
	index_t i;
	double value;
	dataset=dmlfw_row_vec_double_create_new(2);
	if(dataset==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_row_vec_double_set(dataset,0,5.0);
	dmlfw_row_vec_double_set(dataset,1,2.0);
	polynomial_dataset=dmlfw_double_generate_polynomial_dataset(dataset,3);
	if(polynomial_dataset==NULL)
	{
		printf("Unable to generate polynomial dataset\n");
		dmlfw_row_vec_double_destroy(polynomial_dataset);
		return 0;
	}
	polynomial_dataset_size=dmlfw_row_vec_double_get_size(polynomial_dataset);
	for(i=0;i<polynomial_dataset_size;++i)
	{
		value=dmlfw_row_vec_double_get(polynomial_dataset,i);
		printf("%lf ",value);
	}
	printf("\n");
	dmlfw_row_vec_double_destroy(dataset);
	dmlfw_row_vec_double_destroy(polynomial_dataset);
	return 0;
}
