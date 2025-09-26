#ifndef __DMLFW__SCALE__DOUBLE__
#define __DMLFW__SCALE__DOUBLE__
#include<dmlfw_matrix.h>

#define MLFW_MIN_MAX_SCALING_ALGORITHM "min-max"
#define MLFW_Z_SCORE_SCALING_ALGORITHM "z-score"

dmlfw_mat_double * dmlfw_scale_double(char *dataset_file_name,dmlfw_row_vec_string *columns_to_scale,char *parameters_file_name,char *algorithm,dmlfw_mat_double *matrix);

dmlfw_mat_double * dmlfw_scale_double_with_given_parameters(char *dataset_file_name,char *parameters_file_name,char *algorithm,dmlfw_mat_double *matrix);

dmlfw_mat_double * dmlfw_scale_double_min_max(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double **min_max_matrix,dmlfw_mat_double *scaled_matrix);

dmlfw_mat_double * dmlfw_scale_double_with_given_min_max(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double *min_max_matrix,dmlfw_mat_double *scaled_matrix);

dmlfw_mat_double * dmlfw_scale_double_z_score(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double **mean_standard_deviation_matrix,dmlfw_mat_double *scaled_matrix);


dmlfw_mat_double * dmlfw_scale_double_z_score_with_given_mean_standard_deviation(dmlfw_mat_double *matrix,index_t start_row_index,index_t start_column_index,index_t end_row_index,index_t end_column_index,dmlfw_mat_double *mean_standard_deviation_matrix,dmlfw_mat_double *scaled_matrix);

#endif
