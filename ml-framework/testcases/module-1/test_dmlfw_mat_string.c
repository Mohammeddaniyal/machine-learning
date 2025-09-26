#include<stdio.h>
#include<dmlfw_matrix.h>
#include<stdlib.h>
int main()
{
	char *str;
	index_t r,c;
	dimension_t rows,columns;
	dmlfw_mat_string *matrix;
	matrix=dmlfw_mat_string_from_csv("insurance.csv");
	if(matrix==NULL)
	{
		printf("Low memory or unable to load the file\n");
		return 0;
	}
	dmlfw_mat_string_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			dmlfw_mat_string_get(matrix,r,c,&str);
			if(str!=NULL)
			{
				printf("%s ",str);
			}
		}
		printf("\n");
	}
	dmlfw_mat_string_destroy(matrix);
	return 0;
}
