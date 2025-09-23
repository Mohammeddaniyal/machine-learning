#include<stdio.h>
#include<stdlib.h>
#include<mlfw.h>

void __mlfw_double_generate_polynomial_dataset(mlfw_row_vec_double *dataset,index_t i,double accumulated_value,uint8_t exponent,mlfw_forward_list_double *polynomial_dataset)
{
	double yes_value;
	dimension_t dataset_size;
	if(exponent==0) return;
	yes_value=accumulated_value*mlfw_row_vec_double_get(dataset,i);
	mlfw_forward_list_double_insert(polynomial_dataset,yes_value);
	__mlfw_double_generate_polynomial_dataset(dataset,i,yes_value,exponent-1,polynomial_dataset);
	dataset_size=mlfw_row_vec_double_get_size(dataset);
	if(i+1<dataset_size)
	{
	__mlfw_double_generate_polynomial_dataset(dataset,i+1,accumulated_value,exponent,polynomial_dataset);
	}
}
mlfw_row_vec_double * mlfw_double_generate_polynomial_dataset(mlfw_row_vec_double *dataset,uint8_t exponent)
{
	mlfw_forward_list_double *polynomial_dataset=NULL;
	mlfw_row_vec_double *vector=NULL;
	dimension_t polynomial_dataset_size;
	polynomial_dataset=mlfw_forward_list_double_create_new();
	if(mlfw_error()) goto err;
	__mlfw_double_generate_polynomial_dataset(dataset,0,1.0,exponent,polynomial_dataset);
	polynomial_dataset_size=mlfw_forward_list_double_get_size(polynomial_dataset);
	if(polynomial_dataset_size==0) 
	{
		printf("Size of generated polynomial dataset is Zero\n");
		goto err;
	}
	mlfw_forward_list_double_insert(polynomial_dataset,1.0);
	if(mlfw_error()) goto err;
	vector=mlfw_forward_list_double_get_row_vector(polynomial_dataset);
	if(mlfw_error()) goto err;
	mlfw_forward_list_double_destroy(polynomial_dataset);
	return vector;
err:
	mlfw_forward_list_double_destroy(polynomial_dataset);
	mlfw_row_vec_double_destroy(vector);
	return NULL;
}

int main(int argc,char *argv[])
{
	mlfw_mat_double *source_matrix=NULL;
	mlfw_mat_double *target_matrix=NULL;
	dimension_t source_matrix_rows;
	dimension_t source_matrix_columns;
	dimension_t target_matrix_rows;
	dimension_t target_matrix_columns;
	mlfw_row_vec_string *source_matrix_header=NULL;
	mlfw_row_vec_string *target_matrix_header=NULL;
	mlfw_row_vec_double *feature_vector=NULL;
	mlfw_row_vec_double *polynomial_feature_vector=NULL;
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
	source_matrix=mlfw_mat_double_from_csv(source,NULL,&source_matrix_header);
	if(mlfw_error()) goto err;
	mlfw_row_vec_string_destroy(source_matrix_header);
	source_matrix_header=NULL;
	mlfw_mat_double_get_dimensions(source_matrix,&source_matrix_rows,&source_matrix_columns);
	feature_vector=mlfw_row_vec_double_create_new(source_matrix_columns-1);
	if(mlfw_error()) goto err;
	for(r=0;r<source_matrix_rows;++r)
	{
		for(c=0;c<source_matrix_columns-1;++c) // -1 because wwe don't want target value
		{
			feature_value=mlfw_mat_double_get(source_matrix,r,c);
			mlfw_row_vec_double_set(feature_vector,c,feature_value);
		}
		// generate polynomials terms for feature vector
		polynomial_feature_vector=mlfw_double_generate_polynomial_dataset(feature_vector,exponent);
		if(mlfw_error()) goto err;
		if(r==0)
		{
			polynomial_feature_vector_size=mlfw_row_vec_double_get_size(polynomial_feature_vector);
			target_matrix_rows=source_matrix_rows;
			target_matrix_columns=polynomial_feature_vector_size;
			// we don't want bias, hence not adding (1) to columns count
			target_matrix=mlfw_mat_double_create_new(target_matrix_rows,target_matrix_columns);
			if(mlfw_error()) goto err;
		}// if r==0
		 // populate rth row with generated features followed by target values from source matrix
		for(i=1;i<target_matrix_columns;++i) // i=1 because ignore bias
		{
		mlfw_mat_double_set(target_matrix,r,i-1,mlfw_row_vec_double_get(polynomial_feature_vector,i));
		}
		target_value=mlfw_mat_double_get(source_matrix,r,source_matrix_columns-1);
		mlfw_mat_double_set(target_matrix,r,target_matrix_columns-1,target_value);
		mlfw_row_vec_double_destroy(polynomial_feature_vector);
		polynomial_feature_vector=NULL;
		}

		mlfw_row_vec_double_destroy(feature_vector);
		feature_vector=NULL;
		mlfw_mat_double_destroy(source_matrix);
		source_matrix=NULL;

		// code to save the target matrix to target csv file
		
		target_matrix_header=mlfw_row_vec_string_create_new(target_matrix_columns);
		if(mlfw_error()) goto err;
		for(i=0;i<target_matrix_columns;++i)
		{
			sprintf(heading,"%u",i);
			mlfw_row_vec_string_set(target_matrix_header,i,heading);
		}
		mlfw_mat_double_to_csv(target_matrix,target,target_matrix_header);
		mlfw_mat_double_destroy(target_matrix);
		mlfw_row_vec_string_destroy(target_matrix_header);
		printf("%s generated\n",target);
		return 0;
err:

	mlfw_mat_double_destroy(source_matrix);
	mlfw_mat_double_destroy(target_matrix);
	mlfw_row_vec_string_destroy(source_matrix_header);
	mlfw_row_vec_string_destroy(target_matrix_header);
	mlfw_row_vec_double_destroy(feature_vector);
	mlfw_row_vec_double_destroy(polynomial_feature_vector);
	return 0;
}
