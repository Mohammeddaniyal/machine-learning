#include<stdio.h>
#include<stdlib.h>
#include<dmlfw_matrix.h>
#include<dmlfw_scale.h>
#include<dmlfw_utils.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

dmlfw_mat_double * dmlfw_scale_double_min_max(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double **min_max_matrix,dmlfw_mat_double *new_matrix)
{
	double scaled_value;
	double value;
	double *max;
	double *min;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	index_t i;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	dimension_t rows,columns;
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return NULL;
	}
	if(min_max_matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"min_max_matrix");
		return NULL;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	if(start_row_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(end_row_index>=matrix_rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_row_index,"end_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(start_column_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}	
	if(end_column_index>=matrix_columns)
	{ 
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_column_index,"end_column_index",0,matrix_columns-1);
		return NULL;
	}
	if(start_row_index>end_row_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;	
	}
	if(start_column_index>end_column_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}

	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(new_matrix==NULL)
	{
	new_matrix=dmlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(dmlfw_error())
	{
		return NULL;
	}
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns)
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",rows,columns,new_matrix_rows,new_matrix_columns);
			return NULL;
		}
	}
	*min_max_matrix=dmlfw_mat_double_create_new(2,new_matrix_columns);
	if(dmlfw_error())
	{
		return NULL;
	}
	max=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(max==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*new_matrix_columns);
		dmlfw_mat_double_destroy(*min_max_matrix);
		*min_max_matrix=NULL;
		return NULL;
	}
	min=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(min==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*new_matrix_columns);
		free(max);
		dmlfw_mat_double_destroy(*min_max_matrix);
		*min_max_matrix=NULL;
		return NULL;
	}
	i=0;
	for(c=start_column_index;c<=end_column_index;++c)
	{
		min[i]=dmlfw_mat_double_get_minimum(matrix,start_row_index,c,end_row_index,c);
		max[i]=dmlfw_mat_double_get_maximum(matrix,start_row_index,c,end_row_index,c);
		i++;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			value=dmlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-min[new_matrix_c])/(max[new_matrix_c]-min[new_matrix_c]);
			dmlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}
	
	for(i=0;i<new_matrix_columns;++i)
	{
		dmlfw_mat_double_set(*min_max_matrix,0,i,min[i]);
		dmlfw_mat_double_set(*min_max_matrix,1,i,max[i]);
	}
	free(min);
	free(max);
	return new_matrix;
}


dmlfw_mat_double * dmlfw_scale_double_with_given_min_max(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double *min_max_matrix,dmlfw_mat_double *new_matrix)
{
	double scaled_value;
	double value;
	double max;
	double min;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	dimension_t rows,columns;
	dimension_t min_max_rows,min_max_columns;
	
	dmlfw_reset_error();
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return NULL;
	}
	if(min_max_matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"min_max_matrix");
		return NULL;
	}

	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	dmlfw_mat_double_get_dimensions(min_max_matrix,&min_max_rows,&min_max_columns);
	
	if(min_max_rows!=2)
	{
		_dmlfw_set_error(MLFW_INVALID_MIN_MAX_MATRIX_DIMENSIONS_CODE,MLFW_INVALID_MIN_MAX_MATRIX_DIMENSIONS,"min_max_matrix",min_max_rows,min_max_columns,2,end_column_index-start_column_index+1);
	 	return NULL; // reason lec 19 module 1 20:00
	}
	if(min_max_columns!=end_column_index-start_column_index+1)
	{	_dmlfw_set_error(MLFW_INVALID_MIN_MAX_MATRIX_DIMENSIONS_CODE,MLFW_INVALID_MIN_MAX_MATRIX_DIMENSIONS,"min_max_matrix",min_max_rows,min_max_columns,2,end_column_index-start_column_index+1);
	 	return NULL; // reason lec 19 module 1 20:00
	}


	if(start_row_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(end_row_index>=matrix_rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_row_index,"end_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(start_column_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}	
	if(end_column_index>=matrix_columns)
	{ 
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_column_index,"end_column_index",0,matrix_columns-1);
		return NULL;
	}
	if(start_row_index>end_row_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;	
	}
	if(start_column_index>end_column_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}

	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;

	if(min_max_columns!=new_matrix_columns) return NULL; // reason lec 19
	
	if(new_matrix==NULL)
	{
		new_matrix=dmlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(new_matrix_rows!=rows || new_matrix_columns!=columns) 
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",rows,columns,new_matrix_rows,new_matrix_columns);
			return NULL;
		}
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			min=dmlfw_mat_double_get(min_max_matrix,0,new_matrix_c);
			max=dmlfw_mat_double_get(min_max_matrix,1,new_matrix_c);
			value=dmlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-min)/(max-min);
			dmlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}
	return new_matrix;
}



dmlfw_mat_double * dmlfw_scale_double_z_score(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double **mean_standard_deviation_matrix,dmlfw_mat_double *new_matrix)
{
	double scaled_value;
	double value;
	
	double *mean;
	double *standard_deviation;
	
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	index_t i;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	dimension_t rows,columns;
	dmlfw_reset_error();
	
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return NULL;
	}
	if(mean_standard_deviation_matrix==NULL)
	{
	_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"mean_standard_deviation_matrix");
		return NULL;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);

	if(start_row_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(end_row_index>=matrix_rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_row_index,"end_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(start_column_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}	
	if(end_column_index>=matrix_columns)
	{ 
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_column_index,"end_column_index",0,matrix_columns-1);
		return NULL;
	}
	if(start_row_index>end_row_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;	
	}
	if(start_column_index>end_column_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}


	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(new_matrix==NULL)
	{
	new_matrix=dmlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
	if(dmlfw_error()) return NULL;
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(rows!=new_matrix_rows || columns!=new_matrix_columns)
		{
			_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",rows,columns,new_matrix_rows,new_matrix_columns);
			return NULL;
		}
	}
	*mean_standard_deviation_matrix=dmlfw_mat_double_create_new(2,new_matrix_columns);
	if(dmlfw_error())
	{
		return NULL;
	}
	mean=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(mean==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*new_matrix_columns);
		dmlfw_mat_double_destroy(*mean_standard_deviation_matrix);
		*mean_standard_deviation_matrix=NULL;
		return NULL;
	}
	standard_deviation=(double *)malloc(sizeof(double)*new_matrix_columns);
	if(standard_deviation==NULL)
	{
		_dmlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(double)*new_matrix_columns);
		dmlfw_mat_double_destroy(*mean_standard_deviation_matrix);
		*mean_standard_deviation_matrix=NULL;
		free(mean);
		return NULL;
	}
	i=0;
	for(c=start_column_index;c<=end_column_index;++c)
	{
		mean[i]=dmlfw_mat_double_get_mean(matrix,start_row_index,c,end_row_index,c);
		standard_deviation[i]=dmlfw_mat_double_get_standard_deviation(matrix,start_row_index,c,end_row_index,c);
		i++;
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			value=dmlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-mean[new_matrix_c])/(standard_deviation[new_matrix_c]);
			dmlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}

	for(i=0;i<new_matrix_columns;++i)
	{
		dmlfw_mat_double_set(*mean_standard_deviation_matrix,0,i,mean[i]);
		dmlfw_mat_double_set(*mean_standard_deviation_matrix,1,i,standard_deviation[i]);
	}
	free(mean);
	free(standard_deviation);
	return new_matrix;

}


dmlfw_mat_double * dmlfw_scale_double_z_score_with_given_mean_standard_deviation(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double *mean_standard_deviation_matrix,dmlfw_mat_double *new_matrix)
{
	
	double scaled_value;
	double value;
	double mean;
	double standard_deviation;
	index_t r,c;
	index_t new_matrix_r,new_matrix_c;
	dimension_t matrix_rows,matrix_columns;
	dimension_t new_matrix_rows,new_matrix_columns;
	dimension_t rows,columns;
	dimension_t mean_standard_deviation_matrix_rows,mean_standard_deviation_matrix_columns;
	
	dmlfw_reset_error();	
		
	if(matrix==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"matrix");
		return NULL;
	}
	if(mean_standard_deviation_matrix==NULL)
	{
	_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"mean_standard_deviation_matrix");
		return NULL;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	dmlfw_mat_double_get_dimensions(mean_standard_deviation_matrix,&mean_standard_deviation_matrix_rows,&mean_standard_deviation_matrix_columns);

	if(mean_standard_deviation_matrix_rows!=2)// reason lec 19 module 1 20:00
	{	_dmlfw_set_error(MLFW_INVALID_MEAN_AND_STANDARD_DEVIATION_MATRIX_DIMENSIONS_CODE,MLFW_INVALID_MEAN_AND_STANDARD_DEVIATION_MATRIX_DIMENSIONS,"mean_standard_deviation_matrix",mean_standard_deviation_matrix_rows,mean_standard_deviation_matrix_columns,2,end_column_index-start_column_index+1);
	 	return NULL; 
	}
	if(mean_standard_deviation_matrix_rows!=end_column_index-start_column_index+1)
	{	_dmlfw_set_error(MLFW_INVALID_MEAN_AND_STANDARD_DEVIATION_MATRIX_DIMENSIONS_CODE,MLFW_INVALID_MEAN_AND_STANDARD_DEVIATION_MATRIX_DIMENSIONS,"mean_standard_deviation_matrix",mean_standard_deviation_matrix_rows,mean_standard_deviation_matrix_columns,2,end_column_index-start_column_index+1);
	 	return NULL; 
	}

	if(start_row_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(end_row_index>=matrix_rows)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_row_index,"end_row_index",0,matrix_rows-1);
		return NULL;
	}
	if(start_column_index<0)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}	
	if(end_column_index>=matrix_columns)
	{ 
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,end_column_index,"end_column_index",0,matrix_columns-1);
		return NULL;
	}
	if(start_row_index>end_row_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_row_index,"start_row_index",0,matrix_rows-1);
		return NULL;	
	}
	if(start_column_index>end_column_index)
	{
		_dmlfw_set_error(MLFW_INVALID_INDEX_CODE,MLFW_INVALID_INDEX,start_column_index,"start_column_index",0,matrix_columns-1);
		return NULL;
	}

	
	new_matrix_rows=end_row_index-start_row_index+1;
	new_matrix_columns=end_column_index-start_column_index+1;
	if(mean_standard_deviation_matrix_columns!=new_matrix_columns) return NULL; // reason lec 19
	
	if(new_matrix==NULL)
	{
		new_matrix=dmlfw_mat_double_create_new(new_matrix_rows,new_matrix_columns);
		if(dmlfw_error()) return NULL;
	}
	else
	{
		dmlfw_mat_double_get_dimensions(new_matrix,&rows,&columns);
		if(new_matrix_rows!=rows || new_matrix_columns!=columns)
		{
_dmlfw_set_error(MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT_CODE,MLFW_INVALID_MATRIX_CONTAINER_DIMENSIONS_TO_STORE_RESULT,"new_matrix",rows,columns,new_matrix_rows,new_matrix_columns);
			return NULL;
		}
	}
	r=start_row_index;
	for(new_matrix_r=0;new_matrix_r<new_matrix_rows;++new_matrix_r)
	{
		c=start_column_index;
		for(new_matrix_c=0;new_matrix_c<new_matrix_columns;++new_matrix_c)
		{
			mean=dmlfw_mat_double_get(mean_standard_deviation_matrix,0,new_matrix_c);
			standard_deviation=dmlfw_mat_double_get(mean_standard_deviation_matrix,1,new_matrix_c);
			value=dmlfw_mat_double_get(matrix,r,c);
			scaled_value=(value-mean)/(standard_deviation);
			dmlfw_mat_double_set(new_matrix,new_matrix_r,new_matrix_c,scaled_value);
			++c;
		}
		++r;
	}
	return new_matrix;
}


dmlfw_mat_double * dmlfw_scale_double(char *dataset_file_name,dmlfw_row_vec_string *columns_to_scale,char *parameters_file_name,char *algorithm,dmlfw_mat_double *matrix)
{
	dmlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;
	dimension_t matrix_header_size;
	
	index_t i,j;
	dimension_t columns_to_scale_size;

	char *scale_column_name;
	char *column_name;

	dmlfw_mat_double *scaled_column_matrix;
	dmlfw_mat_double *parameters_matrix;
	dmlfw_mat_double *tmp_matrix;

	uint8_t algorithm_code;
	uint8_t MIN_MAX_ALGORITHM=1;
	uint8_t Z_SCORE_ALGORITHM=2;
	
	dmlfw_reset_error();	
	if (dataset_file_name == NULL) 
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "dataset_file_name");
    return NULL;
	}

	if (columns_to_scale == NULL) 	
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "columns_to_scale");
    return NULL;
	}

	if (parameters_file_name == NULL) 
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "parameters_file_name");
    return NULL;
	}

	if (algorithm == NULL) 
	{
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "algorithm");
    return NULL;
	}

	if(dmlfw_strcmp_case_insensitive(algorithm,"min-max")==0)
	{
		algorithm_code=MIN_MAX_ALGORITHM;
	}else if(dmlfw_strcmp_case_insensitive(algorithm,"z-score")==0)
	{
		algorithm_code=Z_SCORE_ALGORITHM;
	}else
	{
		_dmlfw_set_error(MLFW_INVALID_ALGORITHM_NAME_CODE,MLFW_INVALID_ALGORITHM_NAME,"algorithm");
		return NULL;
	}
	
	columns_to_scale_size=dmlfw_row_vec_string_get_size(columns_to_scale);
	if(columns_to_scale_size==0)
	{
		_dmlfw_set_error(MLFW_VECTOR_EMPTY_CODE,MLFW_VECTOR_EMPTY,"columns_to_scale");
		return NULL;
	}
	
	if(algorithm_code==MIN_MAX_ALGORITHM)
	{
		parameters_matrix=dmlfw_mat_double_create_new(2,columns_to_scale_size);
	}else if(algorithm_code==Z_SCORE_ALGORITHM)
	{
		parameters_matrix=dmlfw_mat_double_create_new(2,columns_to_scale_size);
	}

	if(dmlfw_error()) return NULL;
	
	matrix=dmlfw_mat_double_from_csv(dataset_file_name,matrix,&matrix_header);
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(parameters_matrix);
		return NULL;
	}
	
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	matrix_header_size=dmlfw_row_vec_string_get_size(matrix_header);

	scaled_column_matrix=dmlfw_mat_double_create_new(matrix_rows,1);
	
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(parameters_matrix);
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		return NULL;
	}

	// iterate the column the scale vector
	for(i=0;i<columns_to_scale_size;++i)
	{
		// extract column name from columns_to_scale
		dmlfw_row_vec_string_get(columns_to_scale,i,&scale_column_name);
		if(dmlfw_error())
		{
			dmlfw_mat_double_destroy(parameters_matrix);
			dmlfw_mat_double_destroy(matrix);
			dmlfw_row_vec_string_destroy(matrix_header);
			dmlfw_mat_double_destroy(scaled_column_matrix);
			return NULL;
		}
		
		for(j=0;j<matrix_header_size;++j)
		{
			dmlfw_row_vec_string_get(matrix_header,j,&column_name);
			// if fails, release and return NULL
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				free(scale_column_name);
				return NULL;
			}
			if(dmlfw_strcmp_case_insensitive(scale_column_name,column_name)==0)
			{
				// scale j(th) column over here
			if(algorithm_code==MIN_MAX_ALGORITHM)
			{
			dmlfw_scale_double_min_max(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix);
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				free(scale_column_name);
				free(column_name);
				return NULL;
			}
			else
			{
				dmlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
				dmlfw_mat_double_set(parameters_matrix,0,i,dmlfw_mat_double_get(tmp_matrix,0,0));
				dmlfw_mat_double_set(parameters_matrix,1,i,dmlfw_mat_double_get(tmp_matrix,1,0));
				dmlfw_mat_double_destroy(tmp_matrix);	
			}
			} else if(algorithm_code==Z_SCORE_ALGORITHM)
			{
			dmlfw_scale_double_z_score(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix);
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				free(scale_column_name);
				free(column_name);
				return NULL;
			}
			else
			{
				dmlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
				dmlfw_mat_double_set(parameters_matrix,0,i,dmlfw_mat_double_get(tmp_matrix,0,0));
				dmlfw_mat_double_set(parameters_matrix,1,i,dmlfw_mat_double_get(tmp_matrix,1,0));
				dmlfw_mat_double_destroy(tmp_matrix);	
			}
			}
		       	
			free(column_name);
			break;	
			}
			free(column_name);	
		}// inner loop ends
		free(scale_column_name);
		if(j==matrix_header_size) // the column name in columns_to_scale is incorrect
		{
		_dmlfw_set_error(MLFW_INVALID_COLUMN_NAME_CODE,MLFW_INVALID_COLUMN_NAME,scale_column_name);
			dmlfw_mat_double_destroy(parameters_matrix);
			dmlfw_mat_double_destroy(matrix);
			dmlfw_row_vec_string_destroy(matrix_header);
			dmlfw_mat_double_destroy(scaled_column_matrix);
			free(scale_column_name);
			return NULL;
			// release everything created till now and return
		}
		free(scale_column_name);
	} // outer loop ends

	// create parameters file
	dmlfw_mat_double_to_csv(parameters_matrix,parameters_file_name,columns_to_scale);

	// release / destroy
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(parameters_matrix);
	dmlfw_mat_double_destroy(scaled_column_matrix);
return matrix;
}


dmlfw_mat_double * dmlfw_scale_double_with_given_parameters(char *dataset_file_name,char *parameters_file_name,char *algorithm,dmlfw_mat_double *matrix)
{
	dmlfw_row_vec_string *columns_to_scale;
	dmlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;
	dimension_t matrix_header_size;
	
	index_t i,j;
	dimension_t columns_to_scale_size;

	char *scale_column_name;
	char *column_name;

	dmlfw_mat_double *scaled_column_matrix;
	dmlfw_mat_double *parameters_matrix;
	dmlfw_mat_double *tmp_matrix;

	uint8_t algorithm_code;
	uint8_t MIN_MAX_ALGORITHM=1;
	uint8_t Z_SCORE_ALGORITHM=2;
	
	
	dmlfw_reset_error();
	
	if (dataset_file_name == NULL) {
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "dataset_file_name");
    return NULL;
}

if (parameters_file_name == NULL) {
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "parameters_file_name");
    return NULL;
}

if (algorithm == NULL) {
    _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "algorithm");
    return NULL;
}

	
	if(dmlfw_strcmp_case_insensitive(algorithm,"min-max")==0)
	{
		algorithm_code=MIN_MAX_ALGORITHM;
	}else if(dmlfw_strcmp_case_insensitive(algorithm,"z-score")==0)
	{
		algorithm_code=Z_SCORE_ALGORITHM;
	}else
	{
		return NULL;
	}
	
	
	parameters_matrix=dmlfw_mat_double_from_csv(parameters_file_name,NULL,&columns_to_scale);

	

	if(dmlfw_error())
	{
	 	return NULL;
	}
	columns_to_scale_size=dmlfw_row_vec_string_get_size(columns_to_scale);
	matrix=dmlfw_mat_double_from_csv(dataset_file_name,matrix,&matrix_header);
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(parameters_matrix);
		dmlfw_row_vec_string_destroy(columns_to_scale);
		return NULL;
	}
	
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	matrix_header_size=dmlfw_row_vec_string_get_size(matrix_header);

	scaled_column_matrix=dmlfw_mat_double_create_new(matrix_rows,1);
	
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(parameters_matrix);
		dmlfw_row_vec_string_destroy(columns_to_scale);
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		return NULL;
	}
	tmp_matrix=dmlfw_mat_double_create_new(2,1);
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(parameters_matrix);
		dmlfw_row_vec_string_destroy(columns_to_scale);
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		dmlfw_mat_double_destroy(scaled_column_matrix);
		return NULL;
	}
	// iterate the column the scale vector
	for(i=0;i<columns_to_scale_size;++i)
	{
		// extract column name from columns_to_scale
		dmlfw_row_vec_string_get(columns_to_scale,i,&scale_column_name);
		if(dmlfw_error())
		{
			dmlfw_mat_double_destroy(parameters_matrix);
			dmlfw_row_vec_string_destroy(columns_to_scale);
			dmlfw_mat_double_destroy(matrix);
			dmlfw_row_vec_string_destroy(matrix_header);
			dmlfw_mat_double_destroy(scaled_column_matrix);
			dmlfw_mat_double_destroy(tmp_matrix);
			return NULL;
		}
		
		for(j=0;j<matrix_header_size;++j)
		{
			dmlfw_row_vec_string_get(matrix_header,j,&column_name);
			// if fails, release and return NULL
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_row_vec_string_destroy(columns_to_scale);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				dmlfw_mat_double_destroy(tmp_matrix);
				free(scale_column_name);
				return NULL;
			}
			if(dmlfw_strcmp_case_insensitive(scale_column_name,column_name)==0)
			{
				// scale j(th) column over here
			if(algorithm_code==MIN_MAX_ALGORITHM)
			{
				dmlfw_mat_double_set(tmp_matrix,0,0,dmlfw_mat_double_get(parameters_matrix,0,i));
				dmlfw_mat_double_set(tmp_matrix,1,0,dmlfw_mat_double_get(parameters_matrix,1,i));
				dmlfw_scale_double_min_max(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix);
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_row_vec_string_destroy(columns_to_scale);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				dmlfw_mat_double_destroy(tmp_matrix);
				free(scale_column_name);
				free(column_name);
				return NULL;
			}
			else
			{
				dmlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
			}

			} else if(algorithm_code==Z_SCORE_ALGORITHM)
			{
				
				dmlfw_mat_double_set(tmp_matrix,0,0,dmlfw_mat_double_get(parameters_matrix,0,i));
				dmlfw_mat_double_set(tmp_matrix,1,0,dmlfw_mat_double_get(parameters_matrix,1,i));
			dmlfw_scale_double_z_score(matrix,0,j,matrix_rows-1,j,&tmp_matrix,scaled_column_matrix);
			if(dmlfw_error())
			{
				dmlfw_mat_double_destroy(parameters_matrix);
				dmlfw_row_vec_string_destroy(columns_to_scale);
				dmlfw_mat_double_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				dmlfw_mat_double_destroy(scaled_column_matrix);
				dmlfw_mat_double_destroy(tmp_matrix);
				free(scale_column_name);
				free(column_name);
				return NULL;
			}
			else
			{
				dmlfw_mat_double_copy(matrix,scaled_column_matrix,0,j,0,0,matrix_rows-1,0);
			}
		

			}
   			free(column_name);
			break;	
			}
			free(column_name);		
		}// inner loop ends
		if(j==matrix_header_size) // the column name in columns_to_scale is incorrect
		{
		_dmlfw_set_error(MLFW_INVALID_COLUMN_NAME_CODE,MLFW_INVALID_COLUMN_NAME,scale_column_name);
			dmlfw_mat_double_destroy(parameters_matrix);
			dmlfw_mat_double_destroy(matrix);
			dmlfw_row_vec_string_destroy(matrix_header);
			dmlfw_mat_double_destroy(scaled_column_matrix);
			free(scale_column_name);
			return NULL;
			// release everything created till now and return
		}
		free(scale_column_name);
	} // outer loop ends

	// release / destroy
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(tmp_matrix);
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(parameters_matrix);
	dmlfw_mat_double_destroy(scaled_column_matrix);
return matrix;

}
