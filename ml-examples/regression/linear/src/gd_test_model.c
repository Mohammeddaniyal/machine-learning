/**
 * @file gd_test_model.c
 * @brief Tests a linear regression model by predicting and evaluating accuracy using R2 score.
 * @ingroup ml-examples-regression-linear
 * @{
 *
 * @author Mohammed Daniyal
 * @version 1.0
 * @date 2025-09-26
 *
 * This tool loads a test dataset and pre-trained model parameters, makes predictions
 * using ml-framework linear regression prediction functionality, computes the R2 accuracy score,
 * and prints the result. It also handles errors and resource cleanup.
 *
 * Usage:
 *   ./gd_test_model model_csv_file
 *
 * Parameters:
 *   - model_csv_file: Path to CSV file containing saved model parameters.
 */
#include<stdio.h>
#include<dmlfw.h>
#include<stdlib.h>

#define TESTING_DATASET "IceCreamSales_testing_examples.csv"
char *MODEL;
/**
 * @brief Prints the ml-framework error string and exits the program.
 */
void print_error_and_exit()
{
	char error_string[512];
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	exit(0);
}

/**
 * @brief Loads test dataset features X, target values Y, and model parameters.
 *
 * The dataset used is fixed to IceCreamSales_testing_examples.csv.
 * A bias column of all 1.0 is prepended to features.
 *
 * @param[out] x Pointer to features matrix pointer.
 * @param[out] y Pointer to target column vector pointer.
 * @param[out] model Pointer to model parameters vector pointer.
 */
void load_dataset(dmlfw_mat_double **x,dmlfw_column_vec_double **y,dmlfw_column_vec_double **model)
{
	dmlfw_mat_double *matrix=NULL;
	dimension_t matrix_rows,matrix_columns;
	dmlfw_row_vec_string *header=NULL;
	matrix=dmlfw_mat_double_from_csv(TESTING_DATASET,NULL,&header);
	if(dmlfw_error()) 
	{
		print_error_and_exit();
	}
	dmlfw_row_vec_string_destroy(header);
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	*y=dmlfw_mat_double_create_column_vec(matrix,matrix_columns-1,NULL);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		print_error_and_exit();
	}
	dmlfw_mat_double_truncate(&matrix,0,0,matrix_rows-1,matrix_columns-2);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// create column for bias
	dmlfw_mat_double_insert_columns(&matrix,0,1);
	if(dmlfw_error()) 
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	// fill with 1.0
	dmlfw_mat_double_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	dmlfw_mat_double_fill(matrix,0,0,matrix_rows-1,0,1.0);
	*model=dmlfw_column_vec_double_from_csv(MODEL,NULL,&header);
	if(dmlfw_error())
	{
		dmlfw_mat_double_destroy(matrix);
		dmlfw_column_vec_double_destroy(*y);
		*y=NULL;
		print_error_and_exit();
	}
	*x=matrix;
}
/**
 * @brief Main function to run the model testing example.
 *
 * Loads dataset and model, performs prediction, computes accuracy using R2 score,
 * prints accuracy, and manages resources.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return 0 on success or error.
 */
int main(int argc,char *argv[])
{
	char error_string[512];
	char debug_string[512];
	double r2_accuracy_score;
	dmlfw_mat_double *x=NULL;
	dmlfw_column_vec_double *y=NULL;// for parameters
	dmlfw_column_vec_double *predicted_y=NULL;
	dmlfw_column_vec_double *model=NULL;

	if(argc!=2)
	{
		printf("[Usage : gd_test_model model_csv_file]\n");
	       	return 0;	
	}	
	MODEL=argv[1];

	load_dataset(&x,&y,&model); // function defined by framework user
	if(dmlfw_error()) goto err;
	// make predictions
	predicted_y=dmlfw_linear_regression_predict(x,model);
	if(dmlfw_error()) goto err;
	// calculate accuracy score
	r2_accuracy_score=dmlfw_get_r2_score(y,predicted_y);
	if(dmlfw_error()) goto err;

	printf("Accuracy score is : %lf\n",r2_accuracy_score);

	dmlfw_mat_double_destroy(x);
	dmlfw_column_vec_double_destroy(y);
	dmlfw_column_vec_double_destroy(predicted_y);
	dmlfw_column_vec_double_destroy(model);
	return 0;
	err:
	dmlfw_get_error_string(error_string,512);
	dmlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	dmlfw_mat_double_destroy(x);
	dmlfw_column_vec_double_destroy(y);
	dmlfw_column_vec_double_destroy(predicted_y);
	dmlfw_column_vec_double_destroy(model);
	return 0;
}
/** @} */
