#include<stdio.h>
#include<mlfw_matrix.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	mlfw_mat_double *matrix;
	char column_title[11];
	mlfw_row_vec_string *header;
	char *end;
	double min,max;
	int rows,columns;
	index_t x;
	char *filename;

	if(argc!=6)
	{
		printf("Usage [create_matrix_csv_tool rows columns min max filename]\n");
		return 0;
	}
	rows=atoi(argc[1]);
	columns=atoi(argc[2]);
	min=strtod(argc[3],&end);
	max=strtod(argc[4],&end);
	filename=argc[5];

	matrix=mlfw_mat_double_create_new_random_filled(rows,columns,min,max,NULL);
	if(matrix==NULL)
	{
		printf("Unable to create matrix for csv file\n");
		return 0;
	}
	header=mlfw_row_vec_string_create_new(columns);
	if(header==NULL)
	{
		printf("Unable to create matrix for csv file\n");
		mlfw_mat_double_destroy(matrix);
		return 0;
	}
	for(x=1;x<=columns;++x)
	{
		sprintf(column_title,"%u",x);
		mlfw_row_vec_string_set(header,x-1,column_title);
	}
	mlfw_mat_double_to_csv(matrix,filename,header);
	mlfw_mat_double_destroy(matrix);
	mlfw_row_vec_string_destroy(header);
	return 0;
}
