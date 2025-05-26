#include<mlfw_vector.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
typedef struct __mlfw_row_vec_string
{
	char **data;
	dimension_t size;
}mlfw_row_vec_string;

typedef struct __mlfw_column_vec_string
{
	char **data;
	dimension_t size;
}mlfw_column_vec_string;

// row vector functions
mlfw_row_vec_string * mlfw_row_vec_string_create_new(dimension_t columns)
{
	mlfw_row_vec_string *vector;
	index_t i;
	if(columns<=0) return NULL;
	vector=(mlfw_row_vec_string *)malloc(sizeof(mlfw_row_vec_string));
	if(vector==NULL) return NULL;
	vector->data=(char **)malloc(sizeof(char *)*columns);
	if(vector->data==NULL)
	{
		free(vector);
		return 0;
	}
	for(i=0;i<columns;++i)
	{
		vector->data[i]=NULL;
	}
	vector->size=columns;
	return vector;

}
void mlfw_row_vec_string_destroy(mlfw_row_vec_string *vector)
{
	index_t i;
	if(vector==NULL) return;
	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL) free(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}
mlfw_row_vec_string * mlfw_row_vec_string_from_csv(const char *csv_file_name)
{
	mlfw_row_vec_string *vector;
	FILE *file;
	char m;
	dimension_t size;
	index_t i;
	index_t j;
	char string[5001];
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	
	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',') size++;
	}
	size++;
	vector=mlfw_row_vec_string_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	rewind(file); 
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			string[i]='\0';
			vector->data[j]=(char *)malloc(sizeof(char)*(strlen(string)+1));	
			if(vector->data[j]!=NULL)
			{
				strcpy(vector->data[j],string);
			}
			j++;
			i=0;
		}
		else
		{
			string[i]=m;
			++i;
		}
	}

	fclose(file);
	return vector;
}
void mlfw_row_vec_string_to_csv(mlfw_row_vec_string *vector,const char *csv_file_name)
{
	FILE *file;
	dimension_t size;
	index_t i;
	if(vector==NULL || csv_file_name==NULL) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	size=mlfw_row_vec_string_get_size(vector);
	for(i=0;i<size;++i)
	{
		if(vector->data[i]!=NULL)fputs(vector->data[i],file);
		if(i<size-1) fputc(',',file);
		else fputc('\n',file);
	}
	
	fclose(file);
}
void mlfw_row_vec_string_get(mlfw_row_vec_string *vector,index_t index,char **string)
{
	if(vector==NULL || string==NULL) return;
	if(index<0 || index>=vector->size) return;
	*string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
	if(*string==NULL) return;
	strcpy(*string,vector->data[index]);
}
void mlfw_row_vec_string_set(mlfw_row_vec_string *vector,index_t index,char *string)
{
	char *str;
	if(vector==NULL || string==NULL) return;
	if(index<0 || index>=vector->size) return;
	str=(char *)malloc(sizeof(char)*(strlen(string)+1));
	if(str==NULL) return;
	strcpy(str,string);
	vector->data[index]=str;
}
dimension_t mlfw_row_vec_string_get_size(mlfw_row_vec_string *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector)
{
	mlfw_column_vec_string *transposed_vector;
	index_t i;
	if(vector==NULL) return NULL;
	transposed_vector=mlfw_column_vec_string_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	for(i=0;i<vector->size;++i)
	{
		transposed_vector->data[i]=(char *)malloc(sizeof(char)*(strlen(vector->data[i])+1));
		if(transposed_vector->data[i]!=NULL)
		{
			strcpy(transposed_vector->data[i],vector->data[i]);
		}
	}

	return transposed_vector;
}


// column vector functions
mlfw_column_vec_string * mlfw_column_vec_string_create_new(dimension_t rows)
{
	return NULL;
}
void mlfw_column_vec_string_destroy(mlfw_column_vec_string *vector)
{
}
mlfw_column_vec_string * mlfw_column_vec_string_from_csv(const char *csv_file_name)
{
	return NULL;
}
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name)
{
}
void mlfw_column_vec_string_get(mlfw_column_vec_string *vector,index_t index,char **string)
{
}
void mlfw_column_vec_string_set(mlfw_column_vec_string *vector,index_t index,char *string)
{
}
dimension_t mlfw_column_vec_string_get_size(mlfw_column_vec_string *vector)
{
	return 0;
}
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector)
{
	return NULL;
}



