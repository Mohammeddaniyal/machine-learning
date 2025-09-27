/**
 * @file scale_using_parameters_file.c
 * @brief Scales a dataset using given parameters and a specified algorithm.
 * @ingroup ml-tools-scaling
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool loads a dataset CSV and scales it according to parameters specified in 
 * a separate CSV file using the given scaling algorithm, saving the result to a target CSV file.
 *
 * Usage:
 *   ./scale_double_with_given_parameters source_csv target_csv algorithm parameters_csv
 *
 * Parameters:
 *   - source_csv: Path to the input dataset CSV.
 *   - target_csv: Path to write the scaled dataset CSV.
 *   - algorithm: Scaling algorithm name (e.g., "min_max", "z_score").
 *   - parameters_csv: CSV file with scaling parameters to apply.
 *
 */

#include <stdio.h>
#include <dmlfw.h>

/**
 * @brief Main function entry point for scaling tool using given parameters.
 *
 * Loads dataset, applies scaling with specified algorithm and parameters file, 
 * and writes scaled dataset to output CSV.
 *
 * @param argc Command line argument count.
 * @param argv Command line arguments vector.
 * @return 0 on success, prints errors if encountered.
 *
 * Example:
 * @code
 * ./scale_double_with_given_parameters data.csv scaled.csv min_max params.csv
 * @endcode
 */int main(int argc,char *argv[])
{
	dmlfw_mat_double *scaled_matrix=NULL;
	dmlfw_mat_double *matrix=NULL;
	dmlfw_row_vec_string *matrix_header=NULL;
	char *algorithm;
	char *parameters_file;
	char *source;
	char *target;
	char error_string[512];
	
	if(argc!=5)
	{
		printf("Specify [source_csv] [target_csv] [algorithm] [parameters_csv]\n");
		return 0;
	}
	source=argv[1];
	target=argv[2];
	algorithm=argv[3];
	parameters_file=argv[4];
	matrix=dmlfw_mat_double_from_csv(source,NULL,&matrix_header);
	if(dmlfw_error()) goto err;
	dmlfw_mat_double_destroy(matrix);
	matrix=NULL;
	if(dmlfw_error()) goto err;	
	scaled_matrix=dmlfw_scale_double_with_given_parameters(source,parameters_file,algorithm,NULL);
	if(dmlfw_error()) goto err;

	dmlfw_mat_double_to_csv(scaled_matrix,target,matrix_header);
	printf("%s file generated\n",target);
	if(dmlfw_error()) goto err;	
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
err:
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	dmlfw_mat_double_destroy(scaled_matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(matrix);
	return 0;
}
/** @} */
