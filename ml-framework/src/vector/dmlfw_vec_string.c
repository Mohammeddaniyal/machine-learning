#include<dmlfw_vector.h>
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>
extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

typedef struct __dmlfw_row_vec_string
{
	char **data;
	dimension_t size;
}dmlfw_row_vec_string;

typedef struct __dmlfw_column_vec_string
{
	char **data;
	dimension_t size;
}dmlfw_column_vec_string;

// row vector functions
dmlfw_row_vec_string * dmlfw_row_vec_string_create_new(dimension_t columns)
{
	dmlfw_row_vec_string *vector;
	index_t i;
	dmlfw_reset_error();
	if(columns<=0) 
	{
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,columns);
		return NULL;
	}
	vector=(dmlfw_row_vec_string *)malloc(sizeof(dmlfw_row_vec_string));
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_row_vec_string));
		return NULL;
	}
	vector->data=(char **)malloc(sizeof(char *)*columns);
	if(vector->data==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(char *)*columns);
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
void dmlfw_row_vec_string_destroy(dmlfw_row_vec_string *vector)
{
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		return;
	}
	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL) free(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}
dmlfw_row_vec_string * dmlfw_row_vec_string_from_csv(const char *csv_file_name,dmlfw_row_vec_string *vector,dmlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t index;
	index_t header_index;
	dimension_t columns;
	FILE *file;
	char m;
	dimension_t size;
	index_t i;
	index_t c;
	char string[5001];
	dmlfw_reset_error();
	if(csv_file_name==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return NULL;
	}
	if(header==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL) 
	{
	_dmlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
	return NULL;
	}
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
	*header=dmlfw_row_vec_string_create_new(columns);
	if(dmlfw_error()) return NULL;
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
			dmlfw_row_vec_string_set(*header,header_index,header_string);
			header_index++;
			index=0;
			continue;
		}
		if(m=='\n')
		{
			header_string[index]='\0';
			dmlfw_row_vec_string_set(*header,header_index,header_string);
			break;
		}
				
			header_string[index]=m;
			++index;
	}
	// logic to read the first line ends here


	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n') size++;
	}
	if(size!=dmlfw_row_vec_string_get_size(*header))
	{
		_dmlfw_set_error(MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_ROW_VECTOR_HEADER_SIZE,"header",dmlfw_row_vec_string_get_size(*header),columns);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		fclose(file);
		return NULL;
	}
	
	if(vector==NULL)
	{
	vector=dmlfw_row_vec_string_create_new(size);
	if(dmlfw_error())
	{
		fclose(file);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size) 
	{
		_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"csv_file_name[columns[size]]",columns);
		fclose(file);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	rewind(file);
        // skip the first line	
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	i=0;
	c=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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
void dmlfw_row_vec_string_to_csv(dmlfw_row_vec_string *vector,const char *csv_file_name,dmlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return;
	}
	if(header==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return;
	}
	header_size=dmlfw_row_vec_string_get_size(header);
	if(header_size!=vector->size) 
	{
		_dmlfw_set_error(MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_ROW_VECTOR_HEADER_SIZE,"header",header_size,vector->size);
		return;
	}
	file=fopen(csv_file_name,"w");
	if(file==NULL)
	{
		_dmlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE,MLFW_UNABLE_TO_CREATE_FILE,csv_file_name,"csv_file_name");
		return;
	}
	// code to write header
	for(index=0;index<header_size;++index)
	{
		dmlfw_row_vec_string_get(header,index,&ptr);
		if(ptr!=NULL) 
		{
			fputs(ptr,file);
			free(ptr);
		}
		if(index<header_size-1) fputc(',',file);
		else fputc('\n',file);
	}

	// code to write data

	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL)fputs(vector->data[i],file);
		if(i<vector->size-1) fputc(',',file);
		else fputc('\n',file);
	}
	
	fclose(file);
}
void dmlfw_row_vec_string_get(dmlfw_row_vec_string *vector,index_t index,char **string)
{
	unsigned long int allocation_len;
	dmlfw_reset_error();
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		*string=NULL;
		return;
	}
	if(index<0 || index>=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		*string=NULL;
		return;
	}
	if(vector->data[index]==NULL)
	{	
		_dmlfw_set_error(MLFW_NO_STRING_SET_CODE,MLFW_NO_STRING_SET,index);
		*string=NULL;
		return;
	}
	allocation_len=sizeof(char)*(strlen(vector->data[index])+1);
	*string=(char *)malloc(allocation_len);
	if(*string==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
		return;
	}
	strcpy(*string,vector->data[index]);
}
void dmlfw_row_vec_string_set(dmlfw_row_vec_string *vector,index_t index,char *string)
{
	unsigned long int allocation_len;
	dmlfw_reset_error();

	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	if(index<0 || index>=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		return;
	}
	if(vector->data[index]!=NULL) free(vector->data[index]);
	allocation_len=sizeof(char)*(strlen(string)+1);
	vector->data[index]=(char *)malloc(allocation_len);
	if(vector->data[index]!=NULL)
	{      
	strcpy(vector->data[index],string);
	}
	else
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
	}
}
dimension_t dmlfw_row_vec_string_get_size(dmlfw_row_vec_string *vector)
{
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}
dmlfw_column_vec_string * dmlfw_row_vec_string_transpose(dmlfw_row_vec_string *vector,dmlfw_column_vec_string *transposed_vector)
{
	index_t i;
	char *ptr;
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=dmlfw_column_vec_string_create_new(vector->size);
	if(dmlfw_error()) return NULL;
	}
	else
	{
	if(transposed_vector->size!=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"transposed_vector",transposed_vector->size,"vector[size]",vector->size);
	}
	}
	for(i=0;i<vector->size;++i)
	{
		dmlfw_row_vec_string_get(vector,i,&ptr);
		dmlfw_column_vec_string_set(transposed_vector,i,ptr);
		free(ptr);
	}

	return transposed_vector;
}


// column vector functions
dmlfw_column_vec_string * dmlfw_column_vec_string_create_new(dimension_t rows)
{
	dmlfw_column_vec_string *vector;
	index_t i;
	dmlfw_reset_error();
	if(rows<=0)
	{
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,rows);
		return NULL;
	}
	vector=(dmlfw_column_vec_string *)malloc(sizeof(dmlfw_column_vec_string));
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_column_vec_string));
		return NULL;
	}
	vector->data=(char **)malloc(sizeof(char *)*rows);
	if(vector->data==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(char *)*rows);
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
void dmlfw_column_vec_string_destroy(dmlfw_column_vec_string *vector)
{
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL)
	{
		return;
	}
	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL) free(vector->data[i]);
	}
	free(vector->data);
	free(vector);
}
dmlfw_column_vec_string * dmlfw_column_vec_string_from_csv(const char *csv_file_name,dmlfw_column_vec_string *vector,dmlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	index_t index;
	dimension_t columns;
	FILE *file;
	char m;
	dimension_t size;
	index_t i;
	index_t r;
	char string[5001];
	dmlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return NULL;
	}
	if(header==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
	_dmlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
	return NULL;
	}
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
	if(columns!=1)
	{	
		_dmlfw_set_error(MLFW_INVALID_HEADER_SIZE_IN_FILE_CODE,MLFW_INVALID_HEADER_SIZE_IN_FILE,csv_file_name,columns,1);
		fclose(file);
		*header=NULL;
		return NULL;
	}
	*header=dmlfw_row_vec_string_create_new(columns);
	if(dmlfw_error()) return NULL;
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
			dmlfw_row_vec_string_set(*header,header_index,header_string);
			header_index++;
			index=0;
			continue;
		}
		if(m=='\n')
		{
			header_string[index]='\0';
			dmlfw_row_vec_string_set(*header,header_index,header_string);
			break;
		}
				
			header_string[index]=m;
			++index;
	}
	// logic to read the first line ends here

	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m==',' || m=='\n') size++;
	}
	if(vector==NULL)
	{
	vector=dmlfw_column_vec_string_create_new(size);
	if(dmlfw_error())
	{
		fclose(file);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size)
	{	
_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"csv_file_name[rows[size]]",size);
		fclose(file);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
	       	return NULL;
	}
	}
	rewind(file); 
	// skip the first line
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	i=0;
	r=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
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
void dmlfw_column_vec_string_to_csv(dmlfw_column_vec_string *vector,const char *csv_file_name,dmlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return;
	}
	if(header==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return;
	}
	header_size=dmlfw_row_vec_string_get_size(header);
	if(header_size!=1)
	{
		_dmlfw_set_error(MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE,"header",header_size,1);
		return;
	}
	file=fopen(csv_file_name,"w");
	if(file==NULL)
	{	
		_dmlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE,MLFW_UNABLE_TO_CREATE_FILE,csv_file_name,"csv_file_name");
		return;
	}
	// code to write header
	for(index=0;index<header_size;++index)
	{		
		dmlfw_row_vec_string_get(header,index,&ptr);
		if(ptr!=NULL) 
		{
			fputs(ptr,file);
			free(ptr);
		}
		if(index<header_size-1) fputc(',',file);
		else fputc('\n',file);
	}

	// code to write data


	for(i=0;i<vector->size;++i)
	{
		if(vector->data[i]!=NULL)fputs(vector->data[i],file);
		if(i<vector->size-1) fputc(',',file);
		else fputc('\n',file);
	}
	
	fclose(file);

}
void dmlfw_column_vec_string_get(dmlfw_column_vec_string *vector,index_t index,char **string)
{
	unsigned long int allocation_len;
	dmlfw_reset_error();
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		*string=NULL;
		return;
	}
	if(index<0 || index>=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		*string=NULL;
		return;
	}
	if(vector->data[index]==NULL)
	{
		_dmlfw_set_error(MLFW_NO_STRING_SET_CODE,MLFW_NO_STRING_SET,index);
		*string=NULL;
		return;
	}
	allocation_len=sizeof(char)*(strlen(vector->data[index])+1);
	*string=(char *)malloc(allocation_len);
	if(*string==NULL) return;
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
		return;
	}
	strcpy(*string,vector->data[index]);

}
void dmlfw_column_vec_string_set(dmlfw_column_vec_string *vector,index_t index,char *string)
{
	unsigned long int allocation_len;
	dmlfw_reset_error();
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"string");
		return;
	}
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	if(index<0 || index>=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		return;
	}
	if(vector->data[index]!=NULL) free(vector->data[index]);
	allocation_len=sizeof(char)*(strlen(string)+1);
	vector->data[index]=(char *)malloc(allocation_len);
	if(vector->data[index]!=NULL)
	{      
	strcpy(vector->data[index],string);
	}
	else
	{
	_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,allocation_len);
	}
}
dimension_t dmlfw_column_vec_string_get_size(dmlfw_column_vec_string *vector)
{
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}
dmlfw_row_vec_string * dmlfw_column_vec_string_transpose(dmlfw_column_vec_string *vector,dmlfw_row_vec_string *transposed_vector)
{
	index_t i;
	char *ptr;
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=dmlfw_row_vec_string_create_new(vector->size);
	if(dmlfw_error()) return NULL;
	}
	else
	{
	if(transposed_vector->size!=vector->size)
	{	
_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"transposed_vector",transposed_vector->size,"vector[size]",vector->size);
	return NULL;
	}
	}
	for(i=0;i<vector->size;++i)
	{
		dmlfw_column_vec_string_get(vector,i,&ptr);
		dmlfw_row_vec_string_set(transposed_vector,i,ptr);
		free(ptr);
	}

	return transposed_vector;
}



