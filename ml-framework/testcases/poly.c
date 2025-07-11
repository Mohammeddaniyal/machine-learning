#include<stdio.h>
#include<mlfw_vector.h>
#include<mlfw_list.h>


mlfw_row_vec_double mlfw_double_generate_polynomial_dataset(mlfw_row_vec_double *dataset,uint8_t exponent)
{

}

int main()
{
	
	mlfw_row_vec_double *dataset;
	mlfw_row_vec_double *polynomial_dataset;
	index_t i;
	double value;
	dataset=mlfw_row_vec_double_create_new(2);
	if(dataset==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_double_set(dataset,0,5.0);
	mlfw_row_vec_double_set(dataset,1,2.0);
	polynomial_dataset=mlfw_double_generate_polynomial_dataset(dataset,3);
	if(polynomial_dataset==NULL)
	{
		printf("Unable to generate polynomial dataset\n");
		mlfw_row_vec_double_destroy(polynomial_dataset);
		return 0;
	}
	polynomial_dataset_size=mlfw_row_vec_double_get_size(polynomial_dataset);
	for(i=0;i<polynomial_dataset_size;++i)
	{
		value=mlfw_row_vec_double_get(polynomial_dataset,i);
		printf("%lf ",value);
	}
	printf("\n");
	mlfw_row_vec_double_destroy(dataset);
	mlfw_row_vec_double_destroy(polynomial_dataset);
	return 0;
}
