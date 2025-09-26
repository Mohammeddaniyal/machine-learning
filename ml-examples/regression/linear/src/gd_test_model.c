#include<stdio.h>
#include<dmlfw.h>
#include<stdlib.h>

#define TESTING_DATASET "IceCreamSales_testing_examples.csv"
char *MODEL;
void print_error_and_exit()
{
	char error_string[512];
	dmlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	exit(0);
}

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
