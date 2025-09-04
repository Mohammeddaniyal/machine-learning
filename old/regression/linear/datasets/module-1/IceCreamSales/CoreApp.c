#include<stdio.h>
#include<stdio_ext.h> // for linux only
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	double temperature_c;
	double temperature_f;
	double profit;
	mlfw_row_vec_string *header;
	mlfw_column_vec_double *parameters_vector;
	mlfw_row_vec_double *features_vector;
	mlfw_column_vec_double *predicted_value_vector;
	FILE *file;
	if(argc!=2)
	{
		fprintf(stderr,"temperature_in_celsius");
		return -1;
	}
	file=fopen("trained_parameters.csv","r");
	if(file==NULL)
	{
		fprintf(stderr,"trained_parameters.csv does not exists");
		return -1;
	}
	fclose(file);
	temperature_c=strtod(argv[1],NULL);
	temperature_f=(temperature_c*9.0/5.0)+32;
	features_vector=mlfw_row_vec_double_create_new(2);
	if(features_vector==NULL)
	{
		fprintf(stderr,"Low memory");
		return -1;
	}
	mlfw_row_vec_double_set(features_vector,0,1);
	mlfw_row_vec_double_set(features_vector,1,temperature_f);

	parameters_vector=mlfw_column_vec_double_from_csv("trained_parameters.csv",NULL,&header);
	if(parameters_vector==NULL)
	{
		fprintf(stderr,"Low memory");
		mlfw_row_vec_double_destroy(features_vector);
		return -1;
	}
	predicted_value_vector=mlfw_multiply_double_row_vector_with_column_vector(features_vector,parameters_vector,NULL);
	if(predicted_value_vector==NULL)
	{
		fprintf(stderr,"Low memory");
		mlfw_row_vec_double_destroy(features_vector);
		mlfw_column_vec_double_destroy(parameters_vector);
		mlfw_row_vec_string_destroy(header);
		return -1;
	}
	profit=mlfw_column_vec_double_get(predicted_value_vector,0);
	printf("%lf",profit);
	mlfw_row_vec_double_destroy(features_vector);
	mlfw_column_vec_double_destroy(parameters_vector);
	mlfw_column_vec_double_destroy(predicted_value_vector);	
	mlfw_row_vec_string_destroy(header);
	return 0;
}
