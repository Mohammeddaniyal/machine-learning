#include<stdio.h>
#include<stdlib.h>
#include<dmlfw_vector.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>
extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

typedef struct __dmlfw_column_vec_double
{
	double *data;
	dimension_t size;
}dmlfw_column_vec_double;
typedef struct __dmlfw_row_vec_double
{
	double *data;
	dimension_t size;
}dmlfw_row_vec_double;



// column vector function implementations

dmlfw_column_vec_double * dmlfw_column_vec_double_create_new(dimension_t size)
{
	dmlfw_column_vec_double *vector;
	dmlfw_reset_error();
	if(size<=0) 
	{
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	vector=(dmlfw_column_vec_double *)malloc(sizeof(dmlfw_column_vec_double));
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_column_vec_double));
		return NULL;
	}
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		free(vector);
		return NULL;
	}
	vector->size=size;
	return vector;
}
void dmlfw_column_vec_double_destroy(dmlfw_column_vec_double *vector)
{
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		return;
	}
	free(vector->data);
	free(vector);
}
double dmlfw_column_vec_double_get(dmlfw_column_vec_double *vector,index_t index)
{
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	if(index<0 || index>=vector->size) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		return 0.0;
	}
	return vector->data[index];
}
void dmlfw_column_vec_double_set(dmlfw_column_vec_double *vector,index_t index,double value)
{
	dmlfw_reset_error();
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
	vector->data[index]=value;
}
dmlfw_column_vec_double * dmlfw_column_vec_double_create_new_filled(dimension_t size,double value,dmlfw_column_vec_double *vector)
{
	index_t i;
	dmlfw_reset_error();
	if(size<=0)
	{	
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	if(vector==NULL)
	{
		vector=dmlfw_column_vec_double_create_new(size);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		if(vector->size!=size)
		{
			_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"size",size);
			return NULL;
		}
	}
	for(i=0;i<vector->size;++i) vector->data[i]=value;
	return vector;
}
dmlfw_row_vec_double * dmlfw_column_vec_double_transpose(dmlfw_column_vec_double *vector,dmlfw_row_vec_double *transposed_vector)
{
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=dmlfw_row_vec_double_create_new(vector->size);
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
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;

}
dimension_t dmlfw_column_vec_double_get_size(dmlfw_column_vec_double *vector)
{
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}



double dmlfw_column_vec_double_get_mean(dmlfw_column_vec_double *vector)
{
	index_t i;
	double sum;
	double mean;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	sum=0.0;
	for(i=0;i<vector->size;++i)
	{
		sum=sum+vector->data[i];
	}
	mean=sum/(double)vector->size;
	return mean;
}

void dmlfw_column_vec_double_to_csv(dmlfw_column_vec_double *vector,const char *csv_file_name,dmlfw_row_vec_string *header)
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
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
dmlfw_column_vec_double * dmlfw_column_vec_double_from_csv(const char *csv_file_name,dmlfw_column_vec_double *vector,dmlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	index_t index;
	dimension_t columns;
	FILE *file;
	index_t i;
	index_t j;
	dimension_t size;
	char double_string[1025];
	char m;
	char *end;
	double value;
	dmlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
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
		if(m=='\r') continue;
		if(m==',') size++;
	}
	size++; // 10 commas means, 11 elements
	if(vector==NULL)
	{
	vector=dmlfw_column_vec_double_create_new(size);
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
	// skip the first line of the file
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n')
		{
			double_string[i]='\0';
			end=NULL;
			value=strtod(double_string,&end);
			dmlfw_column_vec_double_set(vector,j,value);
			j++;
			i=0;
		}
		else
		{
			double_string[i]=m;
			i++;
		}

	}
	fclose(file);
	return vector;
}
void dmlfw_column_vec_double_copy(dmlfw_column_vec_double *target_vector,dmlfw_column_vec_double *source_vector)
{
	index_t i;
	dmlfw_reset_error();
	if(target_vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"target_vector");
		return;
	}
	if(source_vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"source_vector");
		return;
	}
	if(target_vector->size!=source_vector->size)
	{
		_dmlfw_set_error(MLFW_VECTOR_SIZE_NOT_SAME_CODE,MLFW_VECTOR_SIZE_NOT_SAME,"source_vector","target_vector",source_vector->size,target_vector->size);
		return;
	}
	for(i=0;i<source_vector->size;++i) target_vector->data[i]=source_vector->data[i];
}

// row vector function implementations

dmlfw_row_vec_double * dmlfw_row_vec_double_create_new(dimension_t size)
{
	dmlfw_row_vec_double *vector;
	dmlfw_reset_error();
	if(size<=0) 
	{
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	vector=(dmlfw_row_vec_double *)malloc(sizeof(dmlfw_row_vec_double));
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_row_vec_double));
		return NULL;
	}
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*size);
		free(vector);
		return NULL;
	}
	vector->size=size;
	return vector;
}
void dmlfw_row_vec_double_destroy(dmlfw_row_vec_double *vector)
{
	dmlfw_reset_error();
	if(vector==NULL)
	{
		return;
	}
	free(vector->data);
	free(vector);
}
double dmlfw_row_vec_double_get(dmlfw_row_vec_double *vector,index_t index)
{
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	if(index<0 || index>=vector->size)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,index,"index",0,vector->size-1);
		return 0.0;
	}
	return vector->data[index];
}
void dmlfw_row_vec_double_set(dmlfw_row_vec_double *vector,index_t index,double value)
{
	dmlfw_reset_error();
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
	vector->data[index]=value;
}
dmlfw_row_vec_double * dmlfw_row_vec_double_create_new_filled(dimension_t size,double value,dmlfw_row_vec_double *vector)
{
	index_t i;
	dmlfw_reset_error();
	if(size<=0)
	{
		_dmlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	if(vector==NULL)
	{
	vector=dmlfw_row_vec_double_create_new(size);
	if(dmlfw_error()) return NULL;
	}
	else
	{
	if(vector->size!=size) 
	{
		_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"size",size);
		return NULL;
	}
	}
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
dmlfw_column_vec_double * dmlfw_row_vec_double_transpose(dmlfw_row_vec_double *vector,dmlfw_column_vec_double *transposed_vector)
{
	index_t i;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=dmlfw_column_vec_double_create_new(vector->size);
	if(dmlfw_error()) return NULL;
	}
	else
	{
 	if(transposed_vector->size!=vector->size) 
	{
		_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"transpoed_vector",transposed_vector->size,"vector[size]",vector->size);
		return NULL;	
	}
	}
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;
}
dimension_t dmlfw_row_vec_double_get_size(dmlfw_row_vec_double *vector)
{
	dmlfw_reset_error();
	if(vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}


double dmlfw_row_vec_double_get_mean(dmlfw_row_vec_double *vector)
{
	index_t i;
	double mean,sum;
	dmlfw_reset_error();
	if(vector==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	for(i=0,sum=0.0;i<vector->size;++i) sum+=vector->data[i];
	mean=sum/(double)vector->size;
	return mean;
}
void dmlfw_row_vec_double_to_csv(dmlfw_row_vec_double *vector,const char *csv_file_name,dmlfw_row_vec_string *header)
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
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
dmlfw_row_vec_double * dmlfw_row_vec_double_from_csv(const char *csv_file_name,dmlfw_row_vec_double *vector,dmlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	dimension_t columns;
	dimension_t header_size;
	index_t index;
	FILE *file;
	index_t i;
	index_t j;
	dimension_t size;
	char double_string[1025];
	char m;
	double value;
	char *end;
	dmlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
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
		if(m==',') size++;
	}
	size++; // 10 commas means, 11 elements
	header_size=dmlfw_row_vec_string_get_size(*header);
	if(size!=header_size)
	{
		_dmlfw_set_error(MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_ROW_VECTOR_HEADER_SIZE,"header",header_size,size);
	*header=NULL;
	fclose(file);
	return NULL;
	}
	if(vector==NULL)
	{
	vector=dmlfw_row_vec_double_create_new(size);
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
	_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"csv_file_name[columns[size]]",size);
		fclose(file);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	rewind(file);
	// skip the first line of the file
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m=='\n') break;
	}
	
	i=0;
	j=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' || m=='\n')
		{
			double_string[i]='\0';
			end=NULL;
			value=strtod(double_string,&end);
			dmlfw_row_vec_double_set(vector,j,value);
			j++;
			i=0;
		}
		else
		{
			double_string[i]=m;
			i++;
		}

	}
	fclose(file);
	return vector;

}

void dmlfw_row_vec_double_copy(dmlfw_row_vec_double *target_vector,dmlfw_row_vec_double *source_vector)
{
	index_t i;
	dmlfw_reset_error();
	if(target_vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"target_vector");
		return;
	}
	if(source_vector==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"source_vector");
		return;
	}
	if(target_vector->size!=source_vector->size)
	{
		_dmlfw_set_error(MLFW_VECTOR_SIZE_NOT_SAME_CODE,MLFW_VECTOR_SIZE_NOT_SAME,"source_vector","target_vector",source_vector->size,target_vector->size);
		return;
	}
	for(i=0;i<source_vector->size;++i) target_vector->data[i]=source_vector->data[i];
}
