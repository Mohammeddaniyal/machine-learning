/**
 * @file polynomial_dataset_generator.c
 * @brief Generates a polynomial feature dataset from input CSV.
 * @ingroup ml-tools-dataset
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool reads a numeric dataset CSV, generates polynomial feature expansions 
 * up to a specified exponent for the feature columns (excluding the target column),
 * and writes the expanded polynomial dataset including target values to a CSV file.
 *
 * Core polynomial feature generation is performed recursively with ml-framework row vectors and forward lists.
 *
 * Usage:
 *   ./polynomial_dataset_generator source.csv target.csv exponent
 *
 * Parameters:
 *   - source.csv: Input CSV file containing features and target.
 *   - target.csv: Output CSV file with polynomial feature expansion.
 *   - exponent: Maximum polynomial degree (integer >= 1).
 */
#include<stdio.h>
#include<stdlib.h>
#include<dmlfw.h>

/**
 * @brief Recursive helper to generate polynomial terms from feature vector.
 *
 * @param dataset Feature vector of doubles.
 * @param i Current index in dataset.
 * @param accumulated_value Running product of terms for polynomial.
 * @param exponent Current power level to generate.
 * @param polynomial_dataset Forward list accumulating generated terms.
 */	
void __dmlfw_double_generate_polynomial_dataset(dmlfw_row_vec_double *dataset,index_t i,double accumulated_value,uint8_t exponent,dmlfw_forward_list_double *polynomial_dataset)
{
	double yes_value;
	dimension_t dataset_size;
	if(exponent==0) return;
	yes_value=accumulated_value*dmlfw_row_vec_double_get(dataset,i);
	dmlfw_forward_list_double_insert(polynomial_dataset,yes_value);
	__dmlfw_double_generate_polynomial_dataset(dataset,i,yes_value,exponent-1,polynomial_dataset);
	dataset_size=dmlfw_row_vec_double_get_size(dataset);
	if(i+1<dataset_size)
	{
	__dmlfw_double_generate_polynomial_dataset(dataset,i+1,accumulated_value,exponent,polynomial_dataset);
	}
}
/**
 * @brief Generates polynomial feature vector for given dataset and exponent.
 *
 * @param dataset Feature vector of doubles.
 * @param exponent Maximum polynomial power.
 * @return Vector with generated polynomial features or NULL on error.
 */
dmlfw_row_vec_double * dmlfw_double_generate_polynomial_dataset(dmlfw_row_vec_double *dataset,uint8_t exponent)
{
	dmlfw_forward_list_double *polynomial_dataset=NULL;
	dmlfw_row_vec_double *vector=NULL;
	dimension_t polynomial_dataset_size;
	polynomial_dataset=dmlfw_forward_list_double_create_new();
	if(dmlfw_error()) goto err;
	__dmlfw_double_generate_polynomial_dataset(dataset,0,1.0,exponent,polynomial_dataset);
	polynomial_dataset_size=dmlfw_forward_list_double_get_size(polynomial_dataset);
	if(polynomial_dataset_size==0) 
	{
		printf("Size of generated polynomial dataset is Zero\n");
		goto err;
	}
	dmlfw_forward_list_double_insert(polynomial_dataset,1.0);
	if(dmlfw_error()) goto err;
	vector=dmlfw_forward_list_double_get_row_vector(polynomial_dataset);
	if(dmlfw_error()) goto err;
	dmlfw_forward_list_double_destroy(polynomial_dataset);
	return vector;
err:
	dmlfw_forward_list_double_destroy(polynomial_dataset);
	dmlfw_row_vec_double_destroy(vector);
	return NULL;
}

/**
 * @brief Main entry point for polynomial dataset generation tool.
 *
 * Reads input CSV, generates polynomial features for each row, and outputs expanded CSV.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success or prints error message on failure.
 *
 * Example:
 * @code
 * ./polynomial_dataset_generator source.csv target.csv 3
 * @endcode
 */
int main(int argc,char *argv[])
{
	dmlfw_mat_double *source_matrix=NULL;
	dmlfw_mat_double *target_matrix=NULL;
	dimension_t source_matrix_rows;
	dimension_t source_matrix_columns;
	dimension_t target_matrix_rows;
	dimension_t target_matrix_columns;
	dmlfw_row_vec_string *source_matrix_header=NULL;
	dmlfw_row_vec_string *target_matrix_header=NULL;
	dmlfw_row_vec_double *feature_vector=NULL;
	dmlfw_row_vec_double *polynomial_feature_vector=NULL;
	char heading[11];
	double feature_value;
	double target_value;
	index_t r,c;
	index_t i;
	char *source;
	char *target;
	uint8_t exponent;
	dimension_t  polynomial_feature_vector_size;

	if(argc!=4)
	{
		printf("Specify [source_csv] [target_csv] [exponent]\n");
		return 0;
	}

	source=argv[1];
	target=argv[2];
	exponent=atoi(argv[3]);
	if(exponent<1)
	{
		printf("Invalid exponent argument %d\n",exponent);
		return 0;
	}
	source_matrix=dmlfw_mat_double_from_csv(source,NULL,&source_matrix_header);
	if(dmlfw_error()) goto err;
	dmlfw_row_vec_string_destroy(source_matrix_header);
	source_matrix_header=NULL;
	dmlfw_mat_double_get_dimensions(source_matrix,&source_matrix_rows,&source_matrix_columns);
	feature_vector=dmlfw_row_vec_double_create_new(source_matrix_columns-1);
	if(dmlfw_error()) goto err;
	for(r=0;r<source_matrix_rows;++r)
	{
		for(c=0;c<source_matrix_columns-1;++c) // -1 because wwe don't want target value
		{
			feature_value=dmlfw_mat_double_get(source_matrix,r,c);
			dmlfw_row_vec_double_set(feature_vector,c,feature_value);
		}
		// generate polynomials terms for feature vector
		polynomial_feature_vector=dmlfw_double_generate_polynomial_dataset(feature_vector,exponent);
		if(dmlfw_error()) goto err;
		if(r==0)
		{
			polynomial_feature_vector_size=dmlfw_row_vec_double_get_size(polynomial_feature_vector);
			target_matrix_rows=source_matrix_rows;
			target_matrix_columns=polynomial_feature_vector_size;
			// we don't want bias, hence not adding (1) to columns count
			target_matrix=dmlfw_mat_double_create_new(target_matrix_rows,target_matrix_columns);
			if(dmlfw_error()) goto err;
		}// if r==0
		 // populate rth row with generated features followed by target values from source matrix
		for(i=1;i<target_matrix_columns;++i) // i=1 because ignore bias
		{
		dmlfw_mat_double_set(target_matrix,r,i-1,dmlfw_row_vec_double_get(polynomial_feature_vector,i));
		}
		target_value=dmlfw_mat_double_get(source_matrix,r,source_matrix_columns-1);
		dmlfw_mat_double_set(target_matrix,r,target_matrix_columns-1,target_value);
		dmlfw_row_vec_double_destroy(polynomial_feature_vector);
		polynomial_feature_vector=NULL;
		}

		dmlfw_row_vec_double_destroy(feature_vector);
		feature_vector=NULL;
		dmlfw_mat_double_destroy(source_matrix);
		source_matrix=NULL;

		// code to save the target matrix to target csv file
		
		target_matrix_header=dmlfw_row_vec_string_create_new(target_matrix_columns);
		if(dmlfw_error()) goto err;
		for(i=0;i<target_matrix_columns;++i)
		{
			sprintf(heading,"%u",i);
			dmlfw_row_vec_string_set(target_matrix_header,i,heading);
		}
		dmlfw_mat_double_to_csv(target_matrix,target,target_matrix_header);
		dmlfw_mat_double_destroy(target_matrix);
		dmlfw_row_vec_string_destroy(target_matrix_header);
		printf("%s generated\n",target);
		return 0;
err:

	dmlfw_mat_double_destroy(source_matrix);
	dmlfw_mat_double_destroy(target_matrix);
	dmlfw_row_vec_string_destroy(source_matrix_header);
	dmlfw_row_vec_string_destroy(target_matrix_header);
	dmlfw_row_vec_double_destroy(feature_vector);
	dmlfw_row_vec_double_destroy(polynomial_feature_vector);
	return 0;
}
/** @} */
