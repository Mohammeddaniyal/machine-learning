#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
typedef struct __mlfw_mat_string{
	char ***data;
	dimension_t rows;
	dimension_t columns;
}mlfw_mat_string;
mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns)
{
	mlfw_mat_string *matrix;
	index_t r,k;
	if(rows<=0 || columns<=0) return NULL;
	matrix=(mlfw_mat_double *)malloc(sizeof(mlfw_mat_double));
	if(matrix==NULL) return NULL;
	matrix->data=(double **)malloc(sizeof(double *)*rows);
	if(matrix->data==NULL)
	{
		free(matrix);
		return NULL;
	}
	for(r=0;r<rows;++r)
	{
		matrix->data[r]=(double *)malloc(sizeof(double)*columns);
		if(matrix->data[r]==NULL)
		{
			for(k=0;k<r;++k)
			{
				free(matrix->data[k]);
			}
			free(matrix->data);
			free(matrix);
			return NULL;
		}

	}
	matrix->rows=rows;
	matrix->columns=columns;
	return matrix;
}
void mlfw_mat_string_destroy(mlfw_mat_string *matrix)
{
	index_t r;
	if(matrix==NULL) return;
	for(r=0;r<matrix->rows;++r)
	{
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name)
{
	mlfw_mat_double *matrix;
	int index;
	char m;
	index_t r,c;
	double value;
	char double_string[1025]; //1 extra for \0 (string terminator)
	dimension_t rows,columns;
	FILE *file;
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	rows=0;
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(rows==0)
		{
			if(m==',') columns++;
		}
		if(m=='\n') rows++;
	}
	columns++; // if 7 commas in a line, that means 8 columns
	matrix=mlfw_mat_double_create_new(rows,columns);
       if(matrix==NULL)
       {
	       printf("Unable to create matrix");
	       fclose(file);
	       return NULL;
       }
	rewind(file); // move the internal pointer to the first byte
	// logic to populate matrix starts
	r=0;
	c=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			double_string[index]='\0';
			value=strtod(double_string,NULL);
			index=0;
			matrix->data[r][c]=value;
			c++;
			if(c==matrix->columns)
			{
				c=0;
				r++;
			}
		}
		else
		{
			double_string[index]=m;
			index++;
		}
	}
	fclose(file);
 return matrix;
}
void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string)
{
	double value=0.0;
	if(matrix==NULL) return value;
	if(row<0 || row>=matrix->rows) return value;
	if(column<0 || column>=matrix->columns) return value;
	return matrix->data[row][column];

}
void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char *string)
{
	if(matrix==NULL) return;
	if(row<0 || row>=matrix->rows) return;
	if(column<0 || column>=matrix->columns) return;
	matrix->data[row][column]=value;
}


void mlfw_mat_string_get_dimensions(mlfw_mat_string *matrix,dimension_t *rows,dimension_t *columns)
{
	if(matrix==NULL) return;
	if(rows!=NULL)
	{
		*rows=matrix->rows;
	}
	if(columns!=NULL)
	{
		*columns=matrix->columns;
	}
}

