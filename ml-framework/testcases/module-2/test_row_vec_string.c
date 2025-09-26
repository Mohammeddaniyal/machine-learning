#include<dmlfw_vector.h>
#include<stdio.h>
#include<stdlib.h>
int main()
{
	dmlfw_row_vec_string *vector;
	dmlfw_row_vec_string *csv_vector;
	index_t i;
	dimension_t size;
	char *str;
	vector=dmlfw_row_vec_string_create_new(12);
	if(vector==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	size=dmlfw_row_vec_string_get_size(vector);
	for(i=0;i<size;++i)
	{
		dmlfw_row_vec_string_set(vector,i,"Hello");
	}

	printf("Vector contents\n");
	for(i=0;i<size;i++)
	{
		dmlfw_row_vec_string_get(vector,i,&str);
		if(str!=NULL)
		{
			printf("%s\n",str);
			free(str);
		}
	}


	// now to csv 
	dmlfw_row_vec_string_to_csv(vector,"row_vec.csv");

	
	// from csv
	
	csv_vector=dmlfw_row_vec_string_from_csv("row_vec.csv");
	if(csv_vector==NULL)
	{
		dmlfw_row_vec_string_destroy(vector);
		return 0;
	}
	
	size=dmlfw_row_vec_string_get_size(csv_vector);

	printf("Vector contents\n");
	for(i=0;i<size;i++)
	{
		dmlfw_row_vec_string_get(vector,i,&str);
		if(str!=NULL)
		{
			printf("%s\n",str);
			free(str);
		}
	}
	dmlfw_row_vec_string_destroy(vector);
	dmlfw_row_vec_string_destroy(csv_vector);
	return 0;
}
