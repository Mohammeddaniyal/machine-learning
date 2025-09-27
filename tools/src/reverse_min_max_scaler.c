/**
 * @file reverse_min_max_scaler.c
 * @brief Reverses min-max scaling on specified columns of a dataset CSV.
 * @ingroup ml-tools-scaling
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool reads a scaled dataset CSV and a min-max parameters CSV,
 * reverses the scaling on the last two columns of the dataset using the min and max from the parameters file,
 * and writes the unscaled dataset back to a CSV file.
 *
 * Usage:
 *   ./reverse_scale_output.out input_file output_file min_max_file
 *
 * Parameters:
 *   - input_file: Path to the scaled input CSV dataset.
 *   - output_file: Path to save the unscaled dataset CSV.
 *   - min_max_file: CSV file containing min and max values used for original scaling.
 */

#include <dmlfw_matrix.h>
#include <stdlib.h>
#include <stdio.h>
#include <dmlfw_vector.h>

/**
 * @brief Main entry point for reversing min-max scaling on dataset columns.
 *
 * Reads scaled dataset and min-max parameters, reverses scaling on last two columns,
 * and outputs unscaled dataset CSV.
 *
 * @param argc Number of command line arguments.
 * @param argv Command line arguments vector.
 * @return 0 on success, prints usage on incorrect args.
 *
 * Example:
 * @code
 * ./reverse_scale_output.out scaled.csv unscaled.csv min_max_params.csv
 * @endcode
 */int main(int argc,char *argv[])
{
	char *input_file;
	char *output_file;
	char *min_max_file;
	
	double min;
	double max;
	double scaled_value;
	double value;
	
	dmlfw_mat_double *matrix;
	dmlfw_row_vec_string *matrix_header;
	dimension_t matrix_rows,matrix_columns;

	dmlfw_mat_double *min_max_matrix;
	dmlfw_row_vec_string *min_max_matrix_header;
	dimension_t min_max_rows,min_max_columns;

	index_t r,c;
	if(argc!=4)
	{
		printf("Usage : [reverse_scale_output.out input_file output_file min_max_file]\n");
		return 0;
	}

	input_file=argv[1];
	output_file=argv[2];
	min_max_file=argv[3];

	matrix=dmlfw_mat_double_from_csv(input_file,NULL,&matrix_header);
	if(matrix==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	min_max_matrix=dmlfw_mat_double_from_csv(min_max_file,NULL,&min_max_matrix_header);
	if(min_max_matrix==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	dmlfw_mat_double_get_dimensions(min_max_matrix,&min_max_rows,&min_max_columns);
	
	
	// last column is the output (min and max values)
	// and we only care about the output not input columns
	min=dmlfw_mat_double_get(min_max_matrix,0,min_max_columns-1);
	max=dmlfw_mat_double_get(min_max_matrix,1,min_max_columns-1);


	for(r=0;r<matrix_rows;++r)
	{
		for(c=matrix_columns-2;c<matrix_columns;++c)
		{
			scaled_value=dmlfw_mat_double_get(matrix,r,c);
			value=(scaled_value*(max-min))+min;
			dmlfw_mat_double_set(matrix,r,c,value);
		}
	}
	dmlfw_mat_double_to_csv(matrix,output_file,matrix_header);
	dmlfw_mat_double_destroy(matrix);
	dmlfw_row_vec_string_destroy(matrix_header);
	dmlfw_mat_double_destroy(min_max_matrix);
	dmlfw_row_vec_string_destroy(min_max_matrix_header);
	return 0;
}

/** @} */
