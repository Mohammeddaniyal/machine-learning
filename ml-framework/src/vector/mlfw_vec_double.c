#include<stdio.h>
#include<stdlib.h>
#include<mlfw_vector.h>

extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

typedef struct __mlfw_column_vec_double
{
	double *data;
	dimension_t size;
}mlfw_column_vec_double;
typedef struct __mlfw_row_vec_double
{
	double *data;
	dimension_t size;
}mlfw_row_vec_double;



// column vector function implementations

mlfw_column_vec_double * mlfw_column_vec_double_create_new(dimension_t size)
{
	mlfw_column_vec_double *vector;
	mlfw_reset_error();
	if(size<=0) 
	{
		_mlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	vector=(mlfw_column_vec_double *)malloc(sizeof(mlfw_column_vec_double));
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(mlfw_column_vec_double));
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
void mlfw_column_vec_double_destroy(mlfw_column_vec_double *vector)
{
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	free(vector->data);
	free(vector);
}
double mlfw_column_vec_double_get(mlfw_column_vec_double *vector,index_t index)
{
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	if(index<0 || index>=vector->size) 
	{
		_mlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,"index",0,vector->size-1);
		return 0.0;
	}
	return vector->data[index];
}
void mlfw_column_vec_double_set(mlfw_column_vec_double *vector,index_t index,double value)
{
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	if(index<0 || index>=vector->size)
	{
		_mlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,"index",0,vector->size-1);
		return;
	}
	vector->data[index]=value;
}
mlfw_column_vec_double * mlfw_column_vec_double_create_new_filled(dimension_t size,double value,mlfw_column_vec_double *vector)
{
	index_t i;
	mlfw_reset_error();
	if(size<=0)
	{	
		_mlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	if(vector==NULL)
	{
		vector=mlfw_column_vec_double_create_new(size);
		if(mlfw_error()) return NULL;
	}
	else
	{
		if(vector->size!=size)
		{
			_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"size",size);
			return NULL;
		}
	}
	for(i=0;i<vector->size;++i) vector->data[i]=value;
	return vector;
}
mlfw_row_vec_double * mlfw_column_vec_double_transpose(mlfw_column_vec_double *vector,mlfw_row_vec_double *transposed_vector)
{
	index_t i;
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_row_vec_double_create_new(vector->size);
	if(mlfw_error()) return NULL;
	}
	else
	{
	if(transposed_vector->size!=vector->size) 
	{
		_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"transposed_vector",transposed_vector->size,"vector[size]",vector->size);
		return NULL;
	}
	}	
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;

}
dimension_t mlfw_column_vec_double_get_size(mlfw_column_vec_double *vector)
{
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}



double mlfw_column_vec_double_get_mean(mlfw_column_vec_double *vector)
{
	index_t i;
	double sum;
	double mean;
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
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

void mlfw_column_vec_double_to_csv(mlfw_column_vec_double *vector,char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");	
		return;
	}
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");	
		return;
	}
	if(header==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");	
		return;
	}
	header_size=mlfw_row_vec_string_get_size(header);
	if(header_size!=1) 
	{
		_mlfw_set_error(MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_COLUMN_VECTOR_HEADER_SIZE,"header",header_size,1);
		return;
	}
	file=fopen(csv_file_name,"w");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE,MLFW_UNABLE_TO_CREATE_FILE,csv_file_name,"csv_file_name");
		return;
	}
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


	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_column_vec_double * mlfw_column_vec_double_from_csv(char *csv_file_name,mlfw_column_vec_double *vector,mlfw_row_vec_string **header)
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
	mlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	if(header==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
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
		_mlfw_set_error(MLFW_INVALID_HEADER_SIZE_IN_FILE_CODE,MLFW_INVALID_HEADER_SIZE,csv_file_name,columns,1);
		return NULL;
	}
	*header=mlfw_row_vec_string_create_new(columns);
	if(mlfw_error()) return NULL;
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
	vector=mlfw_column_vec_double_create_new(size);
	if(mlfw_error())
	{
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size)
	{
		_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"csv_file_name[rows[size]]",size);
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
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
			mlfw_column_vec_double_set(vector,j,value);
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


// row vector function implementations

mlfw_row_vec_double * mlfw_row_vec_double_create_new(dimension_t size)
{
	mlfw_row_vec_double *vector;
	mlfw_reset_error();
	if(size<=0) 
	{
		_mlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	vector=(mlfw_row_vec_double *)malloc(sizeof(mlfw_row_vec_double));
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(mlfw_row_vec_double));
		return NULL;
	}
	vector->data=(double *)malloc(sizeof(double)*size);
	if(vector->data==NULL)
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*size);
		free(vector);
		return NULL;
	}
	vector->size=size;
	return vector;
}
void mlfw_row_vec_double_destroy(mlfw_row_vec_double *vector)
{
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return;
	}
	free(vector->data);
	free(vector);
}
double mlfw_row_vec_double_get(mlfw_row_vec_double *vector,index_t index)
{
	mlfw_reset_error()
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	if(index<0 || index>=vector->size)
	{
		_mlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,"index",0,vector->size-1);
		return 0.0;
	}
	return vector->data[index];
}
void mlfw_row_vec_double_set(mlfw_row_vec_double *vector,index_t index,double value)
{
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
	       return;
  	}
	if(index<0 || index>=vector->size)
	{
		_mlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,"index",0,vector->size-1);
	       	return;
	}
	vector->data[index]=value;
}
mlfw_row_vec_double * mlfw_row_vec_double_create_new_filled(dimension_t size,double value,mlfw_row_vec_double *vector)
{
	index_t i;
	mlfw_reset_error();
	if(size<=0)
	{
		_mlfw_set_error(MLFW_INVALID_VECTOR_SIZE_CODE,MLFW_INVALID_VECTOR_SIZE,size);
		return NULL;
	}
	if(vector==NULL)
	{
	vector=mlfw_row_vec_double_create_new(size);
	if(mlfw_error()) return NULL;
	}
	else
	{
	if(vector->size!=size) 
	{
		_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"size",size);
		return NULL;
	}
	}
	for(i=0;i<vector->size;i++) vector->data[i]=value;
	return vector;
}
mlfw_column_vec_double * mlfw_row_vec_double_transpose(mlfw_row_vec_double *vector,mlfw_column_vec_double *transposed_vector)
{
	index_t i;
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return NULL;
	}
	if(transposed_vector==NULL)
	{
	transposed_vector=mlfw_column_vec_double_create_new(vector->size);
	if(mlfw_error()) return NULL;
	}
	else
	{
 	if(transposed_vector->size!=vector->size) 
	{
		_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_CONTAINER_SIZE,"transpoed_vector",transposed_vector->size,"vector[size]",vector->size);
		return NULL;	
	}
	}
	for(i=0;i<vector->size;++i) transposed_vector->data[i]=vector->data[i];
	return transposed_vector;
}
dimension_t mlfw_row_vec_double_get_size(mlfw_row_vec_double *vector)
{
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0;
	}
	return vector->size;
}


double mlfw_row_vec_double_get_mean(mlfw_row_vec_double *vector)
{
	index_t i;
	double mean,sum;
	mlfw_reset_error();
	if(vector==NULL) 
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");
		return 0.0;
	}
	for(i=0,sum=0.0;i<vector->size;++i) sum+=vector->data[i];
	mean=sum/(double)vector->size;
	return mean;
}
void mlfw_row_vec_double_to_csv(mlfw_row_vec_double *vector,char *csv_file_name,mlfw_row_vec_string *header)
{
	index_t index;
	dimension_t header_size;
	char *ptr;
	FILE *file;
	index_t i;
	mlfw_reset_error();
	if(vector==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"vector");	
		return;
	}
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");	
		return;
	}
	if(header==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");	
		return;
	}

	header_size=mlfw_row_vec_string_get_size(header);
	if(header_size!=vector->size)
	{
		_mlfw_set_error(MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_ROW_VECTOR_HEADER_SIZE,"header",header_size,vector->size);
		return;
	}
	file=fopen(csv_file_name,"w");
	if(file==NULL) 
	{
		_mlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE,MLFW_UNABLE_TO_CREATE_FILE,csv_file_name,"csv_file_name");
		return;
	}
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


	for(i=0;i<vector->size;++i)
	{
		fprintf(file,"%lf",vector->data[i]);
		if(i==vector->size-1) fputc('\n',file);
		else fputc(',',file);
	}
	fclose(file);
}
mlfw_row_vec_double * mlfw_row_vec_double_from_csv(char *csv_file_name,mlfw_row_vec_double *vector,mlfw_row_vec_string **header)
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
	mlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	if(header==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"header");
		return NULL;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
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
	*header=mlfw_row_vec_string_create_new(columns);
	if(mlfw_error()) return NULL;
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


	size=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') size++;
	}
	size++; // 10 commas means, 11 elements
	header_size=mlfw_row_vec_string_get_size();
	if(size!=header_size)
	{
		_mlfw_set_error(MLFW_INVALID_ROW_VECTOR_HEADER_SIZE_CODE,MLFW_INVALID_ROW_VECTOR_HEADER_SIZE,"header",header_size,size);
	*header=NULL;
	fclose(file);
	return NULL;
	}
	if(vector==NULL)
	{
	vector=mlfw_row_vec_double_create_new(size);
	if(mlfw_error())
	{
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
		*header=NULL;
		return NULL;
	}
	}
	else
	{
	if(vector->size!=size)
	{
	_mlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector->size,"csv_file_name[columns[size]]",size);
		fclose(file);
		mlfw_row_vec_string_destroy(*header);
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
			mlfw_row_vec_double_set(vector,j,value);
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

