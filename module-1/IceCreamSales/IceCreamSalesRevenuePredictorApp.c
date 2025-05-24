#include<stdio.h>
#include<mlfw_operations.h>
#include<mlfw_vector.h>
int main()
{
	double temperature;
	mlfw_row_vec_double *I;
	mlfw_column_vec_double *m;
	mlfw_column_vec_double *result;
	printf("Enter temperature (Faranhite) : ");
	scanf("%lf",&temperature);
	if(temperature<32 || temperature>212)
	{
		printf("Invalid input, Farenhite value must be in between 32 and 212.\n");
		return 0;
	}

	I=mlfw_row_vec_double_create_new(2);
	if(I==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	mlfw_row_vec_double_set(I,0,1.0);
	mlfw_row_vec_double_set(I,1,temperature);
	
	m=mlfw_column_vec_double_from_csv("model.csv");
	if(m==NULL)
	{
		printf("Unable to load data from model.csv, Low memory\n");
		mlfw_row_vec_double_destroy(I);
		return 0;
	}
	result=mlfw_multiply_double_row_vector_with_column_vector(I,m);
	if(result==NULL)
	{
		printf("Low memory\n");
		mlfw_row_vec_double_destroy(I);
		mlfw_column_vec_double_destroy(m);
		return 0;
	}
	printf("Prediction : %lf\n",mlfw_column_vec_double_get(result,0));
	mlfw_row_vec_double_destroy(I);
	mlfw_column_vec_double_destroy(m);
	mlfw_column_vec_double_destroy(result);
	return 0;
}
