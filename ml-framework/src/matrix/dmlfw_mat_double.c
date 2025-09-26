#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<math.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

typedef struct __dmlfw_mat_double{
	double **data;
	dimension_t rows;
	dimension_t columns;
}dmlfw_mat_double;
dmlfw_mat_double * dmlfw_mat_double_create_new(dimension_t rows,dimension_t columns)
{
	dmlfw_mat_double *matrix;
	index_t r,k;
	dmlfw_reset_error();
	if(rows<=0 || columns<=0) 
	{
	_dmlfw_set_error(MLFW_INVALID_MATRIX_DIMENSION_CODE,MLFW_INVALID_MATRIX_DIMENSION,(uint32_t)rows,(uint32_t)columns);
    return NULL;
	}
	matrix=(dmlfw_mat_double *)malloc(sizeof(dmlfw_mat_double));
	if(matrix==NULL) 
	{	
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(dmlfw_mat_double));
		return NULL;
	}
	matrix->data=(double **)malloc(sizeof(double *)*rows);
	if(matrix->data==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double *)*rows);
		free(matrix);
		return NULL;
	}
	for(r=0;r<rows;++r)
	{
		matrix->data[r]=(double *)malloc(sizeof(double)*columns);
		if(matrix->data[r]==NULL)
		{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*columns);
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
void dmlfw_mat_double_destroy(dmlfw_mat_double *matrix)
{
	index_t r;
	dmlfw_reset_error();
	if(matrix==NULL) 
	{
		return;
	}
	for(r=0;r<matrix->rows;++r)
	{
		free(matrix->data[r]);
	}
	free(matrix->data);
	free(matrix);
}
dmlfw_mat_double * dmlfw_mat_double_from_csv(const char *csv_file_name,dmlfw_mat_double *matrix,dmlfw_row_vec_string **header)
{
	char header_string[1025];
	index_t header_index;
	int index;
	char m;
	index_t r,c;
	double value;
	char double_string[1025]; //1 extra for \0 (string terminator)
	dimension_t rows,columns;
	FILE *file;
	dmlfw_reset_error();
	
	if(csv_file_name==NULL || header==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,(csv_file_name == NULL ? "csv_file_name" : "header"));
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
	if(dmlfw_error())
	{
		fclose(file);
		return NULL;
	} 
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
	if(columns!=dmlfw_row_vec_string_get_size(*header))
	{
		_dmlfw_set_error(MLFW_INVALID_MATRIX_HEADER_SIZE_CODE, MLFW_INVALID_MATRIX_HEADER_SIZE, "header", (uint32_t)dmlfw_row_vec_string_get_size(*header), (uint32_t)columns);
		dmlfw_row_vec_string_destroy(*header);
		*header=NULL;
		fclose(file);
		return NULL;
	}
	if(matrix==NULL)
	{
		matrix=dmlfw_mat_double_create_new(rows,columns);
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
		if(matrix->rows!=rows || matrix->columns!=columns)
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE, MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"matrix",matrix->rows,matrix->columns,rows,columns);
			fclose(file);
			dmlfw_row_vec_string_destroy(*header);
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
double dmlfw_mat_double_get(dmlfw_mat_double *matrix,index_t row,index_t column)
{
	double value=0.0;
	dmlfw_reset_error();
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
		return value;
	}
	if(row<0 || row>=matrix->rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, row, "row", 0, matrix->rows - 1);
		return value;
	}
	if(column<0 || column>=matrix->columns)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, column, "column", 0, matrix->columns - 1);
		return value;
	}
	return matrix->data[row][column];

}
void dmlfw_mat_double_set(dmlfw_mat_double *matrix,index_t row,index_t column,double value)
{
	dmlfw_reset_error();
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix"); 
		return;
	}
	if(row<0 || row>=matrix->rows) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, row, "row", 0, matrix->rows - 1);
		return;
	}
	if(column<0 || column>=matrix->columns) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, column, "column", 0, matrix->columns - 1);
		return;
	}
	matrix->data[row][column]=value;
}


void dmlfw_mat_double_get_dimensions(dmlfw_mat_double *matrix,dimension_t *rows,dimension_t *columns)
{
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix"); 
		return;
	}
	if(rows==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "rows");
		return;
	}
	if(columns==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "columns"); 
		return;
	}
	*rows=matrix->rows;
	*columns=matrix->columns;
}

void dmlfw_mat_double_copy(dmlfw_mat_double *target,dmlfw_mat_double *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index)
{
	index_t target_r;
	index_t target_c;

	index_t source_r;
	index_t source_c;

	dmlfw_reset_error();
	 if(target == NULL) 
    {
        _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "target");
        return;
    }

    if(source == NULL) 
    {
        _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "source");
        return;
    }

 	if(target_row_index < 0 || target_row_index >= target->rows) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, target_row_index, "target_row_index", 0, target->rows - 1);
        return;
    }

    if(target_column_index < 0 || target_column_index >= target->columns) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, target_column_index, "target_column_index", 0, target->columns - 1);
        return;
    }

    if(source_from_row_index < 0 || source_from_row_index >= source->rows) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_from_row_index, "source_from_row_index", 0, source->rows - 1);
        return;
    }

    if(source_from_column_index < 0 || source_from_column_index >= source->columns) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_from_column_index, "source_from_column_index", 0, source->columns - 1);
        return;
    }

    if(source_to_row_index < 0 || source_to_row_index >= source->rows) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_to_row_index, "source_to_row_index", 0, source->rows - 1);
        return;
    }
    if(source_to_column_index < 0 || source_to_column_index >= source->columns) 
    {
        _dmlfw_set_error(MLFW_INVALID_INDEX_CODE, MLFW_INVALID_INDEX, source_to_column_index, "source_to_column_index", 0, source->columns - 1);
        return;
    }

    if(source_from_row_index > source_to_row_index)
    {
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,source_from_row_index, source_to_row_index,0, source->rows - 1);
        return;
    }
	if( source_from_column_index > source_to_column_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,source_from_column_index, source_to_column_index,0, source->columns - 1);
		return;
	}

	target_r=target_row_index;
	source_r=source_from_row_index;
	while(source_r<=source_to_row_index)
	{
		target_c=target_column_index;
		source_c=source_from_column_index;
		while(source_c<=source_to_column_index)
		{
			//precaution measure to avoid out of bounds case
			if(target_r>=0 && target_r<target->rows && target_c>=0 && target_c<target->columns)
			{
				target->data[target_r][target_c]=source->data[source_r][source_c];
			}
			++source_c;
			++target_c;
		}
		++source_r;
		++target_r;
	}
}
void dmlfw_mat_double_fill(dmlfw_mat_double *matrix,index_t from_row_index,index_t from_column_index,index_t to_row_index,index_t to_column_index,double value)
{
	index_t r,c;
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
		return;
	} 
	if(from_row_index>=matrix->rows) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,from_row_index,"from_row_index",0,matrix->rows-1);
		return;
	}
	if(from_column_index>=matrix->columns)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,from_column_index,"from_column_index",0,matrix->columns-1);
		return;
	}
	if(from_row_index<0) from_row_index=0;
	if(from_column_index<0) from_column_index=0;
	if(to_row_index>=matrix->rows) to_row_index=matrix->rows-1;
	if(to_column_index>=matrix->columns) to_column_index=matrix->columns-1;
	if(from_row_index>to_row_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,to_row_index,"to_row_index",0,matrix->rows-1);
		return;
	}
	if(from_column_index>to_column_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,to_column_index,"to_columns_index",0,matrix->columns-1);
		return;
	}
	for(r=from_row_index;r<=to_row_index;++r)
	{
		for(c=from_column_index;c<=to_column_index;++c)
		{
			matrix->data[r][c]=value;
		}
	}
}

dmlfw_column_vec_double * dmlfw_mat_double_create_column_vec(dmlfw_mat_double *matrix,index_t column_index,dmlfw_column_vec_double *vector)
{
	index_t r;
	dimension_t vector_size;
	dmlfw_reset_error();
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return  NULL;
	}
	if(column_index<0 || column_index>=matrix->columns) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,column_index,"column_index",0,matrix->columns-1);
		return NULL;
	}
	if(vector==NULL)
	{
	vector=dmlfw_column_vec_double_create_new(matrix->rows);
	if(dmlfw_error()) return NULL;
	}
	else
	{
		vector_size=dmlfw_column_vec_double_get_size(vector);
		if(vector_size!=matrix->rows) 
		{
			_dmlfw_set_error(MLFW_INVALID_CONTAINER_SIZE_CODE,MLFW_INVALID_CONTAINER_SIZE,"vector",vector_size,"matrix[rows]",matrix->rows);
		}
	}
	for(r=0;r<matrix->rows;++r)
	{
		dmlfw_column_vec_double_set(vector,r,matrix->data[r][column_index]);
	}
	return vector;
}


dmlfw_mat_double * dmlfw_mat_double_shuffle(dmlfw_mat_double *matrix,uint8_t how_many_times_to_shuffle,dmlfw_mat_double *shuffled_matrix)
{
	// idx= (r%(b-a+1))+a
	int r;
	index_t a,b;
	index_t c;
	index_t u;
	index_t idx;
	index_t end_at_index;
	uint8_t j;
	double tmp_var;
	char we_created_container=1;
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		 return NULL;
	}
	if(how_many_times_to_shuffle==0) 
	{
		_dmlfw_set_error(MLFW_INVALID_SHUFFLE_COUNT_CODE,MLFW_INVALID_SHUFFLE_COUNT,how_many_times_to_shuffle);
		return NULL;
	}

	if(shuffled_matrix==NULL)
	{	
	shuffled_matrix=dmlfw_mat_double_create_new(matrix->rows,matrix->columns);
	if(dmlfw_error()) return NULL;
	we_created_container=1;
	}
	else
	{
	if(shuffled_matrix->rows!=matrix->rows || shuffled_matrix->columns!=matrix->columns) 
	{
		_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"shuffled_matrix",shuffled_matrix->rows,shuffled_matrix->columns,matrix->rows,matrix->columns)
		return NULL;
	}
	}
	dmlfw_mat_double_copy(shuffled_matrix,matrix,0,0,0,0,matrix->rows-1,matrix->columns-1);
	if(dmlfw_error())
	{	
		if(we_created_container)
		{
			dmlfw_mat_double_destroy(shuffled_matrix);
			shuffled_matrix=NULL;
		}
		return NULL;
	}
	// b is the lower bound means last index
	b=shuffled_matrix->rows-1; // last row index
	srand(time(NULL));
	end_at_index=shuffled_matrix->rows-3;

	for(j=0;j<how_many_times_to_shuffle;++j)
	{
		for(u=0;u<=end_at_index;++u)
		{
			a=u+1; // upper bound 
			r=rand();
			idx=(r%(b-a+1))+a;
			//swap the data at u and idx row
			for(c=0;c<shuffled_matrix->columns;++c)
			{
				tmp_var=shuffled_matrix->data[u][c];
				shuffled_matrix->data[u][c]=shuffled_matrix->data[idx][c];
				shuffled_matrix->data[idx][c]=tmp_var;
			}
		}
	}
	return shuffled_matrix;
}
void dmlfw_mat_double_to_csv(dmlfw_mat_double *matrix,const char *csv_file_name,dmlfw_row_vec_string *header)
{
	index_t index;
	index_t r,c;
	dimension_t header_size;
	char *ptr;
	char separator;
	FILE *file;
	dmlfw_reset_error();
	if(matrix == NULL)
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix");
    return;
}
if(csv_file_name == NULL)
{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "csv_file_name");
    return;
}
if(header == NULL)
{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "header");
    return;
}

	header_size=dmlfw_row_vec_string_get_size(header);
	if(header_size!=matrix->columns)
	{
		_dmlfw_set_error(MLFW_INVALID_MATRIX_HEADER_SIZE_CODE, MLFW_INVALID_MATRIX_HEADER_SIZE, "header", header_size, (uint32_t)matrix->columns);
		return;
	}
	file=fopen(csv_file_name,"w");
	if(file==NULL) {
		_dmlfw_set_error(MLFW_UNABLE_TO_CREATE_FILE_CODE,MLFW_UNABLE_TO_CREATE_FILE,csv_file_name,"csv_file_name")
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
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			fprintf(file,"%lf",matrix->data[r][c]);
			if(c==matrix->columns-1) // last element
			{
				separator='\n';
			}
			else
			{
				separator=',';
			}
			fputc(separator,file);
		}
	}
	fclose(file);
}

dmlfw_mat_double * dmlfw_mat_double_transpose(dmlfw_mat_double *matrix,dmlfw_mat_double *transposed_matrix)
{
	index_t r,c;
	dmlfw_reset_error();
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
	}
	if(transposed_matrix==NULL)
	{
	transposed_matrix=dmlfw_mat_double_create_new(matrix->columns,matrix->rows);
	if(dmlfw_error()) return NULL;
	}
	else
	{
	if(transposed_matrix->rows!=matrix->columns || transposed_matrix->columns!=matrix->rows) 
	{
		_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"transposed_matrix",transposed_matrix->rows,transposed_matrix->columns,matrix->columns,matrix->rows);
		return NULL;
	}
	}
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			transposed_matrix->data[c][r]=matrix->data[r][c];
		}
	}
	return transposed_matrix;
}

double dmlfw_mat_double_get_minimum(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double minimum;
	index_t r,c;
	dmlfw_reset_error();
	if(matrix==NULL)
 	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
 		return 0.0;
	}
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) 
	{	
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_row_index, start_row_index,0, matrix->rows - 1);
		return 0.0;
	}
	if(start_column_index>end_column_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_column_index, start_column_index,0, matrix->columns - 1);
		return 0.0;
	}
	minimum=matrix->data[start_row_index][start_column_index];
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			if(matrix->data[r][c]<minimum) minimum=matrix->data[r][c];
		}
	}
	return minimum;
}

double dmlfw_mat_double_get_maximum(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double maximum;
	index_t r,c;
	dmlfw_reset_error();
	
	if(matrix==NULL)
 	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
 		return 0.0;
	}
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) 
	{	
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_row_index, start_row_index,0, matrix->rows - 1);
		return 0.0;
	}
	if(start_column_index>end_column_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_column_index, start_column_index,0, matrix->columns - 1);
		return 0.0;
	}
	
	maximum=matrix->data[start_row_index][start_column_index];
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			if(matrix->data[r][c]>maximum) maximum=matrix->data[r][c];
		}
	}
	return maximum;
}

double dmlfw_mat_double_get_mean(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double mean;
	dimension_t elements_count;
	double sum;
	index_t r,c;
	dmlfw_reset_error();

	if(matrix==NULL)
 	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
 		return 0.0;
	}
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) 
	{	
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_row_index, start_row_index,0, matrix->rows - 1);
		return 0.0;
	}
	if(start_column_index>end_column_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_column_index, start_column_index,0, matrix->columns - 1);
		return 0.0;
	}
	sum=0.0;
	elements_count=0;
	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			sum=sum+matrix->data[r][c];
			++elements_count;
		}
	}
	mean=sum/elements_count;
	return mean;

}

double dmlfw_mat_double_get_standard_deviation(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index)
{
	double standard_deviation;
	double mean;
	double sum;
	double square;
	double diff;
	dimension_t elements_count;
	index_t r,c;
	dmlfw_reset_error();
	if(matrix==NULL)
 	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
 		return 0.0;
	}
	if(start_row_index<0) start_row_index=0;
	if(start_column_index<0) start_column_index=0;
	if(end_row_index>=matrix->rows) end_row_index=matrix->rows-1;
	if(end_column_index>=matrix->columns) end_column_index=matrix->columns-1;
	if(start_row_index>end_row_index) 
	{	
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_row_index, start_row_index,0, matrix->rows - 1);
		return 0.0;
	}
	if(start_column_index>end_column_index) 
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_RANGE_CODE, MLFW_INVALID_INDEX_RANGE,start_column_index, start_column_index,0, matrix->columns - 1);
		return 0.0;
	}
	
	mean=dmlfw_mat_double_get_mean(matrix,start_row_index,start_column_index,end_row_index,end_column_index);
	
	sum=0.0;
	elements_count=0;

	for(r=start_row_index;r<=end_row_index;++r)
	{
		for(c=start_column_index;c<=end_column_index;++c)
		{
			diff=matrix->data[r][c]-mean;
			square=diff*diff;
			sum=sum+square;
			++elements_count;
		}
	}
	mean=sum/elements_count;
	standard_deviation=sqrt(mean);
	return standard_deviation;

}


void dmlfw_mat_double_get_training_testing_data(const char *csv_file_name,dmlfw_mat_double **training_data_matrix,dmlfw_mat_double **testing_data_matrix,uint8_t testing_data_percentage)
{
	dmlfw_row_vec_string *header;

	dmlfw_mat_double *matrix;
	dmlfw_mat_double *shuffled_matrix;
	
	dmlfw_mat_double *minor_matrix;
	dmlfw_mat_double *major_matrix;
	
	dimension_t shuffled_matrix_rows;
	dimension_t shuffled_matrix_columns;
	dimension_t minor_rows;
	dimension_t major_rows;

	index_t i,j;	
	dmlfw_reset_error();

	if(csv_file_name==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return;
	}
	
	if(training_data_matrix == NULL)
	{
	*training_data_matrix=NULL;
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "training_data_matrix");
    return;	
	}
	if(testing_data_matrix == NULL)
	{
	*testing_data_matrix=NULL;
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "testing_data_matrix");
    return;
	}
	
	if(testing_data_percentage<=0 || testing_data_percentage>=100)
	{
		_dmlfw_set_error(MLFW_INVALID_PERCENTAGE_CODE, MLFW_INVALID_PERCENTAGE, "testing_data_percentage", (uint32_t)testing_data_percentage);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	matrix=dmlfw_mat_double_from_csv(csv_file_name,NULL,&header);
	if(dmlfw_error())
	{
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	shuffled_matrix=dmlfw_mat_double_shuffle(matrix,3,NULL); // shuffle 3 times
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	dmlfw_mat_double_get_dimensions(shuffled_matrix,&shuffled_matrix_rows,&shuffled_matrix_columns);
	
	minor_rows=(testing_data_percentage*shuffled_matrix_rows)/100;
	major_rows=shuffled_matrix_rows-minor_rows;


	major_matrix=(dmlfw_mat_double *)malloc(sizeof(dmlfw_mat_double));
	if(major_matrix==NULL)
	{
	  _dmlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(dmlfw_mat_double));
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_double_destroy(shuffled_matrix);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	major_matrix->rows=major_rows;
	major_matrix->columns=shuffled_matrix->columns;
	major_matrix->data=(double **)malloc(sizeof(double *)*major_rows);
	if(major_matrix->data==NULL)
	{
		  _dmlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(dmlfw_mat_double *)*major_rows);
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	for(i=0,j=0;i<major_rows;++i,++j)
	{
		major_matrix->data[i]=shuffled_matrix->data[j];
	}
	
	minor_matrix=(dmlfw_mat_double *)malloc(sizeof(dmlfw_mat_double));
	if(minor_matrix==NULL)
	{
		  _dmlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(dmlfw_mat_double));
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix->data);
		free(major_matrix);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	minor_matrix->rows=minor_rows;
	minor_matrix->columns=shuffled_matrix->columns;
	minor_matrix->data=(double **)malloc(sizeof(double *)*minor_rows);
	if(minor_matrix->data==NULL)
	{
		  _dmlfw_set_error(MLFW_LOW_MEMORY_CODE, MLFW_LOW_MEMORY, sizeof(dmlfw_mat_double *)*minor_rows);
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(header);
		dmlfw_mat_double_destroy(shuffled_matrix);
		free(major_matrix->data);
		free(major_matrix);
		free(minor_matrix);
		*training_data_matrix=NULL;
		*testing_data_matrix=NULL;
		return;
	}
	for(i=0;i<minor_rows;++i,++j)
	{
		minor_matrix->data[i]=shuffled_matrix->data[j];
	}

	
	free(shuffled_matrix->data);
	free(shuffled_matrix);

	dmlfw_mat_double_destroy(matrix);
	dmlfw_row_vec_string_destroy(header);
	*training_data_matrix=major_matrix;
	*testing_data_matrix=minor_matrix;	
}


void dmlfw_mat_double_reshape(dmlfw_mat_double **matrix_to_reshape,dimension_t new_rows_count,dimension_t new_columns_count)
{
	dmlfw_mat_double *matrix;
	double **new_data_array;
	double *new_row;
	index_t i,j;
	dmlfw_reset_error();
	if(matrix_to_reshape==NULL) 
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix_to_reshape");
	return;
	}
	if(*matrix_to_reshape == NULL) // added by me not by sir
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "*matrix_to_reshape");
    return;
	}

	if(new_rows_count<=0 || new_columns_count<=0)
	{
		 _dmlfw_set_error(MLFW_INVALID_MATRIX_DIMENSION_CODE, MLFW_INVALID_MATRIX_DIMENSION, (uint32_t)new_rows_count, (uint32_t)new_columns_count);
		dmlfw_mat_double_destroy(*matrix_to_reshape);
		*matrix_to_reshape=NULL;
		return;
	}	
	matrix=*matrix_to_reshape;
	if(matrix->rows==new_rows_count && matrix->columns==new_columns_count) 
	{
		return;
	}
	

	if(new_rows_count>matrix->rows) // increase number of rows
	{
		new_data_array=(double **)realloc(matrix->data,sizeof(double *)*new_rows_count);
		if(new_data_array==NULL)
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double *)*new_rows_count);
			dmlfw_mat_double_destroy(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data=new_data_array;
		for(i=matrix->rows;i<new_rows_count;++i) matrix->data[i]=NULL;
		matrix->rows=new_rows_count;
	}else if(new_rows_count<matrix->rows)
	{
		for(i=new_rows_count;i<matrix->rows;++i)
		{
			free(matrix->data[i]);
		}
		new_data_array=(double **)realloc(matrix->data,sizeof(double *)*new_rows_count);
		if(new_data_array==NULL)
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double *)*new_rows_count);
			for(i=0;i<new_rows_count;++i)
			{
				free(matrix->data[i]);
			}
			free(matrix->data);
			free(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data=new_data_array;
		matrix->rows=new_rows_count;
	}

	// now let's work on columns
	if(matrix->columns==new_columns_count) 
	{
		for(i=0;i<matrix->rows;++i)
		{
			if(matrix->data[i]==NULL)
			{
				matrix->data[i]=(double *)malloc(sizeof(double)*matrix->columns);
				if(matrix->data[i]==NULL)
				{
					_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*matrix->columns);
					for(j=0;j<i;++j)
					{
						free(matrix->data[j]);
					}
					free(matrix->data);
					free(matrix);
					*matrix_to_reshape=NULL;
					return;
				}
			}
		}
		return;
	}
	// logic to reshape content of each row
	for(i=0;i<matrix->rows;++i)
	{
		new_row=(double *)realloc(matrix->data[i],sizeof(double)*new_columns_count);
		if(new_row==NULL)
		{
			_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*new_columns_count);
			dmlfw_mat_double_destroy(matrix);
			*matrix_to_reshape=NULL;
			return;
		}
		matrix->data[i]=new_row;
	}
	matrix->columns=new_columns_count;

}

void dmlfw_mat_double_right_shift(dmlfw_mat_double *matrix,dimension_t how_many_places_to_shift)
{
	index_t r,c,new_c;
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return;
	}
	if(how_many_places_to_shift <= 0) 
	{
    _dmlfw_set_error(MLFW_INVALID_SHIFT_COUNT_CODE, MLFW_INVALID_SHIFT_COUNT, how_many_places_to_shift);
    return;
	}
	if(matrix->columns<how_many_places_to_shift) how_many_places_to_shift=matrix->columns;
	for(r=0;r<matrix->rows;++r)
	{
		c=matrix->columns-1;
		while(1) // reason of infinite loop M2L19 51:00, because c is unsigned it will never become -ve
		{
			new_c=how_many_places_to_shift+c;
			if(new_c<matrix->columns)
			{
				matrix->data[r][new_c]=matrix->data[r][c];
			}
			matrix->data[r][c]=0.0;
			if(c==0) break;
			c--;
		}
	}
}
void dmlfw_mat_double_left_shift(dmlfw_mat_double *matrix,dimension_t how_many_places_to_shift)
{
	int64_t r,c,new_c; // reason for int64_t M2 L21 2:34
	if(matrix==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return;
	}
	if(how_many_places_to_shift <= 0) 
	{
    	_dmlfw_set_error(MLFW_INVALID_SHIFT_COUNT_CODE, MLFW_INVALID_SHIFT_COUNT, how_many_places_to_shift);
    	return;
	}	if(matrix->columns<how_many_places_to_shift) how_many_places_to_shift=matrix->columns;
	for(r=0;r<matrix->rows;++r)
	{
		c=0;
		while(c<matrix->columns)
		{
			new_c=c-how_many_places_to_shift;
			if(new_c>=0)
			{
				matrix->data[r][new_c]=matrix->data[r][c];
			}
			matrix->data[r][c]=0.0;
			c++;
		}
	}
}


dmlfw_mat_double * dmlfw_mat_double_create_identity_matrix(dimension_t rows)
{
	dmlfw_mat_double *matrix;
	index_t r,c;
	if(rows <= 0) 
    {
        _dmlfw_set_error(MLFW_INVALID_MATRIX_DIMENSION_CODE, MLFW_INVALID_MATRIX_DIMENSION, rows, rows);
        return NULL;
    }
    matrix = dmlfw_mat_double_create_new(rows, rows);
    if(dmlfw_error()) 
    {
        return NULL;
    }
	for(r=0;r<matrix->rows;++r)
	{
		for(c=0;c<matrix->columns;++c)
		{
			matrix->data[r][c]=(double)(r==c);
		}
	}
	return matrix;
}


dmlfw_mat_double * dmlfw_mat_double_clone(dmlfw_mat_double *matrix_to_clone,dmlfw_mat_double *new_matrix)
{
	dmlfw_mat_double *matrix;
	char we_created_container=0;
	if(matrix_to_clone==NULL) 
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix_to_clone");
		return NULL;
	}
	if(new_matrix==NULL)
	{
	matrix=dmlfw_mat_double_create_new(matrix_to_clone->rows,matrix_to_clone->columns);
	if(dmlfw_error()) return NULL;
	we_created_container=1;
	}
	else
	{
	if(new_matrix->rows!=matrix_to_clone->rows || new_matrix->columns!=matrix_to_clone->columns) 
	{
		_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",new_matrix->rows,new_matrix->columns,matrix_to_clone->rows,matrix_to_clone->columns);
		return NULL;
	}
	matrix=new_matrix;
	}
	dmlfw_mat_double_copy(matrix,matrix_to_clone,0,0,0,0,matrix_to_clone->rows-1,matrix_to_clone->columns-1); 
	if(dmlfw_error()) 
	{
		if(we_created_container)
		{
			dmlfw_mat_double_destroy(new_matrix);
			new_matrix=NULL;
		}
		return NULL;
	}
	return matrix;
}



dmlfw_mat_double * dmlfw_mat_double_inverse(dmlfw_mat_double *matrix_to_inverse,dmlfw_mat_double *new_matrix)
{
	dmlfw_mat_double *identity_matrix;
	dmlfw_mat_double *matrix;
	index_t pivot_row_index,pivot_column_index;
	index_t r,c;
	index_t i;
	index_t row_index_of_largest;
	double pivot_value;
	double largest;
	double value;
	double eliminate_value;
	double multiply_by;
	dmlfw_reset_error();
	if(matrix_to_inverse == NULL) 
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "matrix_to_inverse");
    return NULL;
	}
	if(matrix_to_inverse->rows != matrix_to_inverse->columns) 
	{
    _dmlfw_set_error(MLFW_NOT_SQUARE_MATRIX_CODE, MLFW_NOT_SQUARE_MATRIX, "matrix_to_inverse");
    return NULL;
	}
	matrix=dmlfw_mat_double_clone(matrix_to_inverse,NULL);
	if(dmlfw_error()) return NULL;
	if(new_matrix==NULL)
	{
		identity_matrix=dmlfw_mat_double_create_identity_matrix(matrix->rows);
		if(dmlfw_error())
		{
			dmlfw_mat_double_destroy(matrix);
			return NULL;
		}
	}
	else
	{
		if(new_matrix->rows!=matrix->rows || new_matrix->columns!=matrix->columns)
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",new_matrix->rows,new_matrix->columns,matrix->rows,matrix->columns);
			dmlfw_mat_double_destroy(matrix);
			return NULL;
		}

		// maje this new_matrix as identity matrix by filling 1 and 0's using nested loop
		for(r=0;r<new_matrix->rows;++r)
		{
			for(c=0;c<new_matrix->columns;++c)
			{
				if(r==c) new_matrix->data[r][c]=1.0;
				else new_matrix->data[r][c]=0.0;
			}
		}
		identity_matrix=new_matrix;
	}
	for(r=0;r<matrix->rows;++r)
	{
		pivot_row_index=r;
		pivot_column_index=r;
		pivot_value=matrix->data[pivot_row_index][pivot_column_index];
		largest=pivot_value;
		if(largest<0) largest=largest*(-1); // making it as absolute value
		row_index_of_largest=r;
		// loop to find the index of largest absolute below r
		for(i=r+1;i<matrix->rows;++i)
		{
			value=matrix->data[i][pivot_column_index];
			if(value<0) value=value*(-1);
			if(largest<value)
			{
				largest=value;
				row_index_of_largest=i;
			}
		}	
		// if r!=index_of_largest then swap rows of matrix as well as identity matrix
		if(r!=row_index_of_largest)
		{
			for(c=0;c<matrix->columns;++c)
			{
		// swap rows in matrix
		value=matrix->data[pivot_row_index][c];
		matrix->data[pivot_row_index][c]=matrix->data[row_index_of_largest][c];
		matrix->data[row_index_of_largest][c]=value;
		// swap rows in identity_matrix
		value=identity_matrix->data[pivot_row_index][c];
		identity_matrix->data[pivot_row_index][c]=identity_matrix->data[row_index_of_largest][c];
		identity_matrix->data[row_index_of_largest][c]=value;
			}
		}
		
		// make the pivot point value as 1 by dividing all elements of rth row by 
		// pivot point value
		
		pivot_value=matrix->data[pivot_row_index][pivot_column_index];
		
		if(pivot_value==0)
		{
			_dmlfw_set_error(MLFW_SINGULAR_MATRIX_CODE, MLFW_SINGULAR_MATRIX);
			dmlfw_mat_double_destroy(matrix);
			if(new_matrix==NULL)
			{
				dmlfw_mat_double_destroy(identity_matrix);
			}
			return NULL;
		}

		for(c=0;c<matrix->columns;++c)
		{
		matrix->data[r][c]=matrix->data[r][c]/pivot_value;
		identity_matrix->data[r][c]=identity_matrix->data[r][c]/pivot_value;
		}

		// pivot value will becomes 1 
//		pivot_value=matrix->data[pivot_row_index][pivot_column_index];


		// loop to elimintate all values above rth row in pivot_column_index column
		// Note that identity matrix should also be updated
		
		// formule will be Rn -> Rn - (Pivot_Row * (Rn_value_to_eliminate/Pivot_point_value)))

		for(i=0;i<r;i++)
		{
			eliminate_value=matrix->data[i][pivot_column_index];
			multiply_by=eliminate_value; // so ignoring settng up pivot point
						     // value as denominator
						     // as pivot point value is 1
			for(c=0;c<matrix->columns;++c)
			{
				value=matrix->data[i][c];
				value=value-(matrix->data[r][c]*multiply_by);
				matrix->data[i][c]=value;
			
				value=identity_matrix->data[i][c];
				value=value-(identity_matrix->data[r][c]*multiply_by);
				identity_matrix->data[i][c]=value;
			}
		
		}

		// loop to elimintate all values below rth row in pivot_column_index column
		// Note that identity matrix should also be updated
	
		for(i=r+1;i<matrix->rows;i++)
		{
			eliminate_value=matrix->data[i][pivot_column_index];
			multiply_by=eliminate_value; // so ignoring settng up pivot point
						     // value as denominator
						     // as pivot point value is 1
			for(c=0;c<matrix->columns;++c)
			{
				value=matrix->data[i][c];
				value=value-(matrix->data[r][c]*multiply_by);
				matrix->data[i][c]=value;
			
				value=identity_matrix->data[i][c];
				value=value-(identity_matrix->data[r][c]*multiply_by);
				identity_matrix->data[i][c]=value;
			}
		
		}

	}// outer loop to traverse on all rows ends here
	dmlfw_mat_double_destroy(matrix);
	return identity_matrix; // identity matrix is now the inverse of matrix_to_inverse
}


dmlfw_mat_double * dmlfw_mat_double_create_new_random_filled(dimension_t rows,dimension_t columns,double min,double max,dmlfw_mat_double *new_matrix)
{
	dmlfw_mat_double *matrix;
	index_t r,c;
	double value;
	dimension_t new_matrix_rows,new_matrix_columns;
	dmlfw_reset_error();
if(rows <= 0 || columns <= 0)
{
    _dmlfw_set_error(MLFW_INVALID_MATRIX_DIMENSION_CODE, MLFW_INVALID_MATRIX_DIMENSION, rows, columns);
    return NULL;
}

if(min > max)
{
    _dmlfw_set_error(MLFW_INVALID_MIN_MAX_RANGE_CODE, MLFW_INVALID_MIN_MAX_RANGE, min, max);
    return NULL;
}


	if(new_matrix==NULL)
	{
		matrix=dmlfw_mat_double_create_new(rows,columns);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&new_matrix_rows,&new_matrix_columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns) 
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",new_matrix_rows,new_matrix_columns,rows,columns);
		}
		matrix=new_matrix;
	}
	if(min==max)
	{
		for(r=0;r<rows;++r)
		{
			for(c=0;c<columns;++c)
			{
				matrix->data[r][c]=min;
			}	
		}
	}
	else
	{
		srand(time(NULL));
		for(r=0;r<matrix->rows;++r)
		{
			for(c=0;c<matrix->columns;++c)
			{
				value=rand();
				value=((value*(max-min))/RAND_MAX)+min;
				matrix->data[r][c]=value;
			}
		}
	}
	return matrix;
}

void dmlfw_mat_double_truncate(dmlfw_mat_double **matrix,index_t from_row_index,index_t from_column_index,index_t to_row_index,index_t to_column_index)
{
	dimension_t rows,columns;
	dmlfw_mat_double *new_matrix;
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return;
	}
	if(*matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"*matrix");
		return;
	}
	if(to_row_index>=(*matrix)->rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,to_row_index,"*matrix",0,(index_t)(*matrix)->rows-1);
		return;
	}
	if(to_column_index>=(*matrix)->rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,to_column_index,"*matrix",0,(index_t)(*matrix)->columns-1);
		return;
	}
	rows=to_row_index-from_row_index+1;
	columns=to_column_index-from_column_index+1;
	new_matrix=dmlfw_mat_double_create_new(rows,columns);
	if(dmlfw_error()) return;
	dmlfw_mat_double_copy(new_matrix,*matrix,0,0,from_row_index,from_column_index,to_row_index,to_column_index);
	dmlfw_mat_double_destroy(*matrix);
	*matrix=new_matrix;

}

void dmlfw_mat_double_insert_columns(dmlfw_mat_double **matrix,index_t at_index,dimension_t number_of_columns)
{
	dimension_t rows,columns;
	dmlfw_mat_double *new_matrix;
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return;
	}
	if(*matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"*matrix");
		return;
	}
	if(at_index>=(*matrix)->columns)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,at_index,"*matrix",0,(index_t)(*matrix)->columns-1);
		return;
	}
	if(number_of_columns==0) return;
	rows=(*matrix)->rows;
	columns=(*matrix)->columns+number_of_columns;
	new_matrix=dmlfw_mat_double_create_new(rows,columns);
	if(dmlfw_error()) return;
	if(at_index>0 && at_index<(*matrix)->columns)
	{
		dmlfw_mat_double_copy(new_matrix,*matrix,0,0,0,0,(*matrix)->rows-1,at_index-1);
		dmlfw_mat_double_copy(new_matrix,*matrix,0,at_index+number_of_columns,0,at_index,(*matrix)->rows-1,(*matrix)->columns-1);
	}
	else if(at_index==0)
	{
		dmlfw_mat_double_copy(new_matrix,*matrix,0,0+number_of_columns,0,0,(*matrix)->rows-1,(*matrix)->columns-1);
	}
	else if(at_index==(*matrix)->columns)
	{
		dmlfw_mat_double_copy(new_matrix,*matrix,0,0,0,0,(*matrix)->rows-1,(*matrix)->columns-1);
	}
	dmlfw_mat_double_destroy(*matrix);
	*matrix=new_matrix;
}

dmlfw_mat_double * dmlfw_mat_double_get_block_from_csv(const char *csv_file_name,dmlfw_mat_double *matrix,uint64_t from_row,uint64_t how_many_rows,long *pos)
{
	int index;
	double value;
	char double_string[1025]; // 1 extra for \0 (string terminator)
	double **new_data;
	long file_size;
	long tmp_pos;
	uint64_t r,k;
	index_t c;
	dimension_t rows,columns;
	char m;
	FILE *file;
	dmlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return NULL;
	}
	if(from_row==0)
	{
		_dmlfw_set_error(MLFW_INVALID_FROM_ROW_CODE,MLFW_INVALID_FROM_ROW,from_row);
		return NULL;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_dmlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
		return NULL;
	}
	fseek(file,0,SEEK_END);
	file_size=ftell(file);
	rewind(file);
	if(pos!=NULL && (*pos>=file_size ||  *pos<0)) *pos=0;
	// read the header and count columns
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		else if(m==',') columns++;
		else if(m=='\n') break;
	}
	columns++; // n commas means n+1 columns

	if(feof(file))
	{
		fclose(file);
		_dmlfw_set_error(MLFW_NO_ROWS_TO_READ_CODE,MLFW_NO_ROWS_TO_READ,csv_file_name);
		return NULL;
	}
	if(pos==NULL || *pos==0)
	{
		// reach the from_row(th) row
		r=1;
		while(1)
		{
			if(r==from_row) break;
			m=fgetc(file);
			if(feof(file)) break;
			if(m=='\r') continue;
			else if(m=='\n') r++;
		}
		if(feof(file))
		{
			fclose(file);
			_dmlfw_set_error(MLFW_NO_ROWS_TO_READ_CODE,MLFW_NO_ROWS_TO_READ,csv_file_name);
			return NULL;
		}
		tmp_pos=ftell(file);
	}
	else
	{
		tmp_pos=*pos;
	}
	// now tmp_pos represents the position of the first byte of from_row(th) row
	if(tmp_pos!=ftell(file))
	{
		fseek(file,tmp_pos,SEEK_SET);
	}
	rows=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		else if(m=='\n') rows++;
		if(rows==how_many_rows) break;
	}
	if(rows==0) // ideally this will not happen
	{
		fclose(file);
		_dmlfw_set_error(MLFW_NO_ROWS_TO_READ_CODE,MLFW_NO_ROWS_TO_READ,csv_file_name);
		return NULL;
	}
	fseek(file,tmp_pos,SEEK_SET);
	// value of rows may be less than how_many_rows
	how_many_rows=rows;
	if(matrix==NULL)
	{
		matrix=dmlfw_mat_double_create_new(rows,columns);
		if(dmlfw_error()) 
		{
			fclose(file);
			return NULL;
		}
	}
	else
	{
		if(matrix->columns!=columns)
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"matrix",matrix->rows,matrix->columns,rows,columns);
			fclose(file);
			return NULL;
		}
		if(matrix->rows!=rows)
		{
			// destroy internals and create new internal
			// but keep the same wrapper structure
			// first lets create new internal, if succeds, then destroy old ones
			new_data=(double **)malloc(sizeof(double *)*rows);
			if(new_data==NULL)
			{
				_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double *)*rows);
				return NULL;
			}
			for(r=0;r<rows;++r)
			{
				new_data[r]=(double *)malloc(sizeof(double)*columns);
				if(new_data[r]==NULL)
				{
					for(k=0;k<r;++k)
					{
						free(new_data[k]);
					}
					free(new_data);
				_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*columns);
				return NULL;
					
				}
			}
			// new internals created, release the old ones
			for(r=0;r<matrix->rows;++r)
			{
				free(matrix->data[r]);
			}
			free(matrix->data);
			matrix->data=new_data;
			matrix->rows=rows;
			matrix->columns=columns;
		}
	}
	// now lets read rows number of rows and populate the matrix
	
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
			double_string[index]='\0';
			value=strtod(double_string,NULL);
			index=0;
			matrix->data[r][c]=value;
			c++;
			if(c==matrix->columns)
			{
				r++;
				c=0;
				if(r==rows) break; // we are done reading required row
			}
		}
		else
		{
			double_string[index]=m;
			index++;
		}
	}
	tmp_pos=ftell(file);
	if(pos!=NULL) *pos=tmp_pos;
	fclose(file);
	return matrix;
}
