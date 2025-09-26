#include<stdio.h>
#include<dmlfw_operations.h>
#include<dmlfw_vector.h>
int main()
{
	double temperature;
	dmlfw_row_vec_double *I;
	dmlfw_column_vec_double *m;
	dmlfw_column_vec_double *result;
	printf("Enter temperature (Faranhite) : ");
	scanf("%lf",&temperature);
	if(temperature<32 || temperature>212)
	{
		printf("Invalid input, Farenhite value must be in between 32 and 212.\n");
		return 0;
	}

	I=dmlfw_row_vec_double_create_new(2);
	if(I==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_row_vec_double_set(I,0,1.0);
	dmlfw_row_vec_double_set(I,1,temperature);
	
	m=dmlfw_column_vec_double_from_csv("model.csv");
	if(m==NULL)
	{
		printf("Unable to load data from model.csv, Low memory\n");
		dmlfw_row_vec_double_destroy(I);
		return 0;
	}
	result=dmlfw_multiply_double_row_vector_with_column_vector(I,m);
	if(result==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_double_destroy(I);
		dmlfw_column_vec_double_destroy(m);
		return 0;
	}
	printf("Prediction : %lf\n",dmlfw_column_vec_double_get(result,0));
	dmlfw_row_vec_double_destroy(I);
	dmlfw_column_vec_double_destroy(m);
	dmlfw_column_vec_double_destroy(result);
	return 0;
}
