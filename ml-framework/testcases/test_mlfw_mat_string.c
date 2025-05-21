#include<stdio.h>
#include<mlfw_matrix.h>
#include<stdlib.h>
int main()
{
	char *str;
	index_t r,c;
	dimension_t rows,columns;
	mlfw_mat_string *matrix;
	matrix=mlfw_mat_string_from_csv("insurance.csv");
	if(matrix==NULL)
	{
		printf("Low memory or unable to load the file\n");
		return 0;
	}
	mlfw_mat_string_get_dimenions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&str);
			if(str!=NULL)
			{
				printf("%s ",str);
			}
		}
		printf("\n");
	}
	mlfw_mat_string_destroy(matrix);
	return 0;
}
