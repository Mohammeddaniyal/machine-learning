/**
 * @file scale_dataset.c
 * @brief Generic scaling tool for datasets using specified algorithm and parameters.
 * @ingroup ml-tools-scaling
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool reads a CSV dataset, scales specified columns with a chosen scaling algorithm 
 * and parameters from a CSV file, then writes the scaled dataset to a target CSV file.
 *
 * Usage:
 *   ./scale_double source.csv target.csv algorithm parameters_csv column1 column2 ...
 *
 * Parameters:
 *   - source.csv: Path to input dataset CSV file.
 *   - target.csv: Path to output scaled dataset CSV file.
 *   - algorithm: Scaling algorithm name (e.g., "min_max", "z_score").
 *   - parameters_csv: CSV file containing scaling parameters.
 *   - column1, column2, ... : List of column names to scale.
 */

#include <stdio.h>
#include <dmlfw.h>

/**
 * @brief Main entry point for the generic scaling tool.
 *
 * Loads dataset, sets columns to scale, applies scaling algorithm, and saves output.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success, prints error on failure.
 *
 * Example:
 * @code
 * ./scale_double data.csv scaled.csv min_max params.csv Age Salary
 * @endcode
 */int main(int argc,char *argv[])
{
	dmlfw_mat_double *scaled_matrix=NULL;
	dmlfw_row_vec_string *columns_to_scale=NULL;
	dmlfw_mat_double *matrix=NULL;
	dmlfw_row_vec_string *matrix_header=NULL;
	char *algorithm;
	char *parameters_file;
	char *source;
	char *target;
	char error_string[512];
	index_t i;
	dimension_t columns_to_scale_size;
	
	if(argc<=5)
	{
		printf("Specify [source csv] [target csv] [algorithm] [parameters_csv] columns_to_scale ....\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	algorithm=argv[3];
	parameters_file=argv[4];
	columns_to_scale_size=argc-5; // -5 because (program_name,source,target,algorithm and parameters_csv)
	matrix=dmlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(dmlfw_error()) goto err;
	dmlfw_mat_double_destroy(matrix);
	matrix=NULL;
	columns_to_scale=dmlfw_row_vec_string_create_new(columns_to_scale_size);
	if(dmlfw_error()) goto err;	
	for(i=5;i<argc;++i)
	{
		dmlfw_row_vec_string_set(columns_to_scale,i-5,argv[i]);
	}
	scaled_matrix=dmlfw_scale_double(source,columns_to_scale,parameters_file,algorithm,NULL);
	if(dmlfw_error()) goto err;

	dmlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
err:
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	dmlfw_row_vec_string_destroy(columns_to_scale);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
}
/** @} */
