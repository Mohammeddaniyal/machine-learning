#ifndef __MLFW__MAT__DOUBLE__
#define __MLFW__MAT__DOUBLE__

struct __mlfw_mat_double;
typedef struct __mlfw_mat_double mlfw_mat_double;

mlfw_mat_double * mlfw_mat_double_create_new(dimension_t rows,dimension_t columns);
void mlfw_mat_double_destroy(mlfw_mat_double *matrix);
// if user pass NULL in 3rd arg so our function will create a new matrix and return it
// or if not then it will use the arg matrix refrence and return it
mlfw_mat_double * mlfw_mat_double_from_csv(const char *csv_file_name,mlfw_mat_double *matrix);

double mlfw_mat_double_get(mlfw_mat_double *matrix,index_t row,index_t column);
void mlfw_mat_double_set(mlfw_mat_double *matrix,index_t row,index_t column,double value);

void mlfw_mat_double_get_dimensions(mlfw_mat_double *matrix,dimension_t *rows,dimension_t *columns);

void mlfw_mat_double_copy(mlfw_mat_double *target,mlfw_mat_double *source,index_t target_row_index,index_t target_column_index,index_t source_from_row_index,index_t source_from_column_index,index_t source_to_row_index,index_t source_to_column_index);

void mlfw_mat_double_fill(mlfw_mat_double *matrix,index_t from_row_index,index_t from_column_index,index_t to_row_index,index_t to_column_index,double value);

mlfw_column_vec_double * mlfw_mat_double_create_column_vec(mlfw_mat_double *matrix,index_t column_index,mlfw_column_vec_double *vector);

mlfw_mat_double * mlfw_mat_double_shuffle(mlfw_mat_double *matrix,uint8_t how_many_times_to_shuffle,mlfw_mat_double *shuffled_matrix);
void mlfw_mat_double_to_csv(mlfw_mat_double *matrix,const char *csv_file_name);

mlfw_mat_double * mlfw_mat_double_transpose(mlfw_mat_double *matrix,mlfw_mat_double *transposed_matrix);

double mlfw_mat_double_get_minimum(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index);

double mlfw_mat_double_get_maximum(mlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index);
#endif
