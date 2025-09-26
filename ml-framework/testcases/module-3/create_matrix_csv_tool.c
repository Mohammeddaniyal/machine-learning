#include<stdio.h>
#include<dmlfw_matrix.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	dmlfw_mat_double *matrix;
	char column_title[11];
	dmlfw_row_vec_string *header;
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
	rows=atoi(argv[1]);
	columns=atoi(argv[2]);
	min=strtod(argv[3],&end);
	max=strtod(argv[4],&end);
	filename=argv[5];

	matrix=dmlfw_mat_double_create_new_random_filled(rows,columns,min,max,NULL);
	if(matrix==NULL)
	{
		printf("Unable to create matrix for csv file\n");
		return 0;
	}
	header=dmlfw_row_vec_string_create_new(columns);
	if(header==NULL)
	{
		printf("Unable to create matrix for csv file\n");
		dmlfw_mat_double_destroy(matrix);
		return 0;
	}
	for(x=1;x<=columns;++x)
	{
		sprintf(column_title,"%u",x);
		dmlfw_row_vec_string_set(header,x-1,column_title);
	}
	dmlfw_mat_double_to_csv(matrix,filename,header);
	dmlfw_mat_double_destroy(matrix);
	dmlfw_row_vec_string_destroy(header);
	return 0;
}
