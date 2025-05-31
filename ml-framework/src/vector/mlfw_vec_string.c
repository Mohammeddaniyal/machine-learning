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
mlfw_row_vec_string * mlfw_row_vec_string_from_csv(const char *csv_file_name,mlfw_row_vec_string *vector)
{
	FILE *file;
	char m;
	dimension_t size;
	index_t i;
	index_t c;
	char string[5001];
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	
	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n') size++;
	}
	if(vector==NULL)
	{
	vector=mlfw_row_vec_string_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size) return NULL;
	}
	rewind(file); 
	i=0;
	c=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			string[i]='\0';
			vector->data[c]=(char *)malloc(sizeof(char)*(strlen(string)+1));	
			if(vector->data[c]!=NULL)
			{
				strcpy(vector->data[c],string);
			}
			c++;
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
	index_t i;
	if(vector==NULL || csv_file_name==NULL) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL)fputs(vector->data[i],file);
		if(i<vector->size-1) fputc(',',file);
		else fputc('\n',file);
	}
	
	fclose(file);
}
void mlfw_row_vec_string_get(mlfw_row_vec_string *vector,index_t index,char **string)
{
	if(string==NULL) return;
	if(vector==NULL)
	{
		*string=NULL;
		return;
	}
	if(index<0 || index>=vector->size)
	{
		*string=NULL;
		return;
	}
	if(vector->data[index]==NULL)
	{
		*string=NULL;
		return;
	}
	*string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
	if(*string==NULL) return;
	strcpy(*string,vector->data[index]);
}
void mlfw_row_vec_string_set(mlfw_row_vec_string *vector,index_t index,char *string)
{
	if(vector==NULL || string==NULL) return;
	if(index<0 || index>=vector->size) return;
	if(vector->data[index]!=NULL) free(vector->data[index]);
	vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
	if(vector->data[index]!=NULL)
	{      
	strcpy(vector->data[index],string);
	}
}
dimension_t mlfw_row_vec_string_get_size(mlfw_row_vec_string *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}
mlfw_column_vec_string * mlfw_row_vec_string_transpose(mlfw_row_vec_string *vector,mlfw_column_vec_string *transposed_vector)
{
	index_t i;
	char *ptr;
	if(vector==NULL) return NULL;
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_column_vec_string_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	}
	else
	{
	if(transposed_vector->size!=vector->size) return NULL;
	}
	for(i=0;i<vector->size;++i)
	{
		mlfw_row_vec_string_get(vector,i,&ptr);
		mlfw_column_vec_string_set(transposed_vector,i,ptr);
		free(ptr);
	}

	return transposed_vector;
}


// column vector functions
mlfw_column_vec_string * mlfw_column_vec_string_create_new(dimension_t rows)
{
	mlfw_column_vec_string *vector;
	index_t i;
	if(rows<=0) return NULL;
	vector=(mlfw_column_vec_string *)malloc(sizeof(mlfw_column_vec_string));
	if(vector==NULL) return NULL;
	vector->data=(char **)malloc(sizeof(char *)*rows);
	if(vector->data==NULL)
	{
		free(vector);
		return 0;
	}
	for(i=0;i<rows;++i)
	{
		vector->data[i]=NULL;
	}
	vector->size=rows;
	return vector;

}
void mlfw_column_vec_string_destroy(mlfw_column_vec_string *vector)
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
mlfw_column_vec_string * mlfw_column_vec_string_from_csv(const char *csv_file_name,mlfw_column_vec_string *vector)
{
	FILE *file;
	char m;
	dimension_t size;
	index_t i;
	index_t r;
	char string[5001];
	if(csv_file_name==NULL) return NULL;
	file=fopen(csv_file_name,"r");
	if(file==NULL) return NULL;
	
	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n') size++;
	}
	if(vector==NULL)
	{
	vector=mlfw_column_vec_string_create_new(size);
	if(vector==NULL)
	{
		fclose(file);
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size) return NULL;
	}
	rewind(file); 
	i=0;
	r=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n')
		{
			string[i]='\0';
			vector->data[r]=(char *)malloc(sizeof(char)*(strlen(string)+1));	
			if(vector->data[r]!=NULL)
			{
				strcpy(vector->data[r],string);
			}
			r++;
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
void mlfw_column_vec_string_to_csv(mlfw_column_vec_string *vector,const char *csv_file_name)
{
	FILE *file;
	index_t i;
	if(vector==NULL || csv_file_name==NULL) return;
	file=fopen(csv_file_name,"w");
	if(file==NULL) return;
	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL)fputs(vector->data[i],file);
		if(i<vector->size-1) fputc(',',file);
		else fputc('\n',file);
	}
	
	fclose(file);

}
void mlfw_column_vec_string_get(mlfw_column_vec_string *vector,index_t index,char **string)
{
	if(string==NULL) return;
	if(vector==NULL)
	{
		*string=NULL;
		return;
	}
	if(index<0 || index>=vector->size)
	{
		*string=NULL;
		return;
	}
	if(vector->data[index]==NULL)
	{
		*string=NULL;
		return;
	}
	*string=(char *)malloc(sizeof(char)*(strlen(vector->data[index])+1));
	if(*string==NULL) return;
	strcpy(*string,vector->data[index]);

}
void mlfw_column_vec_string_set(mlfw_column_vec_string *vector,index_t index,char *string)
{
	if(vector==NULL || string==NULL) return;
	if(index<0 || index>=vector->size) return;
	if(vector->data[index]!=NULL) free(vector->data[index]);
	vector->data[index]=(char *)malloc(sizeof(char)*(strlen(string)+1));
	if(vector->data[index]!=NULL)
	{      
	strcpy(vector->data[index],string);
	}
}
dimension_t mlfw_column_vec_string_get_size(mlfw_column_vec_string *vector)
{
	if(vector==NULL) return 0;
	return vector->size;
}
mlfw_row_vec_string * mlfw_column_vec_string_transpose(mlfw_column_vec_string *vector,mlfw_row_vec_string *transposed_vector)
{
	index_t i;
	char *ptr;
	if(vector==NULL) return NULL;
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_row_vec_string_create_new(vector->size);
	if(transposed_vector==NULL) return NULL;
	}
	else
	{
	if(transposed_vector->size!=vector->size) return NULL;
	}
	for(i=0;i<vector->size;++i)
	{
		mlfw_column_vec_string_get(vector,i,&ptr);
		mlfw_row_vec_string_set(transposed_vector,i,ptr);
		free(ptr);
	}

	return transposed_vector;
}



