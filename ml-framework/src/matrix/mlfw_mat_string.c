#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
typedef struct __mlfw_mat_string{
	char ***data;
	dimension_t rows;
	dimension_t columns;
}mlfw_mat_string;
mlfw_mat_string * mlfw_mat_string_create_new(dimension_t rows,dimension_t columns)
{
	mlfw_mat_string *matrix;
	index_t r,k,c;
	if(rows<=0 || columns<=0) return NULL;
	matrix=(mlfw_mat_string *)malloc(sizeof(mlfw_mat_string));
	if(matrix==NULL) return NULL;
	matrix->data=(char ***)malloc(sizeof(char **)*rows);
	if(matrix->data==NULL)
	{
		free(matrix);
		return NULL;
	}
	for(r=0;r<rows;++r)
	{
		matrix->data[r]=(char **)malloc(sizeof(char *)*columns);
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
		else
		{
			for(c=0;c<columns;c++) matrix->data[r][c]=NULL; // very veryy IMP reason lec18 12:15
		}

	}
	matrix->rows=rows;
	matrix->columns=columns;
	return matrix;
}
void mlfw_mat_string_destroy(mlfw_mat_string *matrix)
{
	index_t r,c;
	if(matrix==NULL) return;
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			if(matrix->data[r][c]!=NULL) free(matrix->data[r][c]);
		}
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}
mlfw_mat_string * mlfw_mat_string_from_csv(const char *csv_file_name,mlfw_mat_string *matrix,mlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;	
	int index;
	char m;
	index_t r,c;
	char string[5001]; //1 extra for \0 (string terminator)
	dimension_t rows,columns;
	FILE *file;
	if(csv_file_name==NULL || header==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;

	// logic to read the first line starts here
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') columns++;
		if(m=='\n') break;
		
	}
	columns++; // if 0 commas, then 1 column, if 3 commas then 4 columns
	*header=mlfw_row_vec_string_create_new(columns);
	if(*header==NULL) return NULL;
	rewind(file);
	index=0;
	header_index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			header_index++;
			index=0;
			continue;
		}
		if(m=='\n')
		{
			header_string[index]='\0';
			mlfw_row_vec_string_set(*header,header_index,header_string);
			break;
		}
				
			header_string[index]=m;
			++index;
	}
	// logic to read the first line ends here


	rows=0;
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(rows==0)
		{
			if(m==',') columns++;
		}
		if(m=='\n') rows++;
	}
	columns++; // if 7 commas in a line, that means 8 columns
	if(columns!=mlfw_row_vec_string_get_size(*header)) 
	{
		mlfw_row_vec_string_destroy(*header);
		fclose(file);
		*header=NULL;
		return NULL;
	}
	if(matrix==NULL)
	{
		matrix=mlfw_mat_string_create_new(rows,columns);
      		if(matrix==NULL)
      		 {
	       		fclose(file);
	       		mlfw_row_vec_string_destroy(*header);
			*header=NULL;
			return NULL;
       		}
	}
	else
	{
		if(matrix->rows!=rows || matrix->columns!=columns) 
		{
			fclose(file);
			mlfw_row_vec_string_destroy(*header);
			*header=NULL;
			return NULL;
		}
	}
	rewind(file); // move the internal pointer to the first byte
	// skip the first line of the file
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	// logic to populate matrix starts
	r=0;
	c=0;
	index=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n')
		{
			string[index]='\0';
			matrix->data[r][c]=(char *)malloc(sizeof(char)*(strlen(string)+1)); 
			if(matrix->data[r][c]!=NULL)
			{
					strcpy(matrix->data[r][c],string);
			}
			index=0;
			c++;
			if(c==matrix->columns)
			{
				c=0;
				r++;
			}
		}
		else
		{
			string[index]=m;
			index++;
		}
	}
	fclose(file);
 return matrix;
}
void mlfw_mat_string_to_csv(mlfw_mat_string *matrix,const char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t r,c;
	if(matrix==NULL || csv_file_name==NULL || header==NULL) return;
	header_size=mlfw_row_vec_string_get_size(header);
	if(header_size!=matrix->columns) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	// code to write header
		
		for(index=0;index<header_size;++index)
		{
			mlfw_row_vec_string_get(header,index,&ptr);
			if(ptr!=NULL) 
			{
				fputs(ptr,file);
				free(ptr);
			}
			if(index<header_size-1) fputc(',',file);
			else fputc('\n',file);
		}

	// code to write data

	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			if(matrix->data[r][c]!=NULL)fputs(matrix->data[r][c],file);
			if(c==matrix->columns-1) fputc('\n',file);
			else fputc(',',file);
		}
	}
	fclose(file);
}
void mlfw_mat_string_get(mlfw_mat_string *matrix,index_t row,index_t column,char **string)
{
	if(string==NULL) return;
	if(matrix==NULL)
	{
		*string=NULL;
	       	return;
	}
	if(row<0 || row>=matrix->rows)
	{
		*string=NULL;
		return;
	}
	if(column<0 || column>=matrix->columns) 
	{
		*string=NULL;
		return;
	}
	if(matrix->data[row][column]==NULL)
	{
		*string=NULL;
		return;
	}
	*string=(char *)malloc(sizeof(char)*(strlen(matrix->data[row][column])+1));
	if(*string==NULL) return;
	strcpy(*string,matrix->data[row][column]);
}
void mlfw_mat_string_set(mlfw_mat_string *matrix,index_t row,index_t column,char *string)
{
	if(matrix==NULL || string==NULL) return;
	if(row<0 || row>=matrix->rows) return;
	if(column<0 || column>=matrix->columns) return;
	if(matrix->data[row][column]!=NULL) free(matrix->data[row][column]);
	matrix->data[row][column]=(char *)malloc(sizeof(char)*(strlen(string)+1));
	if(matrix->data[row][column])
	{
		strcpy(matrix->data[row][column],string);

	}
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

mlfw_mat_string * mlfw_mat_string_transpose(mlfw_mat_string *matrix,mlfw_mat_string *transposed_matrix)
{
	index_t r,c;
	char *ptr;
	if(matrix==NULL) return NULL;
	if(transposed_matrix==NULL)
	{
	transposed_matrix=mlfw_mat_string_create_new(matrix->columns,matrix->rows);
	if(transposed_matrix==NULL) return NULL;
	}
	else
	{
	if(transposed_matrix->rows!=matrix->rows || transposed_matrix->columns!=matrix->columns) return NULL;
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&ptr);
			mlfw_mat_string_set(transposed_matrix,c,r,ptr);
			free(ptr);
		}
	}
	return transposed_matrix;
}
