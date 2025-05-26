#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdlib.h>
#include<stdio.h>
// global variable
char *DATASET_FILE_NAME=NULL;
char *RESULTS_FILE_NAME=NULL;
char *PARAMETERS_FILE_NAME=NULL;
void test_it()
{
	index_t r,c;
	FILE *result_file;
	mlfw_mat_double *dataset;
	dimension_t dataset_rows;
	dimension_t dataset_columns;
	
	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;


	dataset=mlfw_mat_double_from_csv(DATASET_FILE_NAME);
	if(dataset==NULL)
	{
		printf("Unable to load dataset from %s\n",DATASET_FILE_NAME);
		return;
	}
	mlfw_mat_double_get_dimensions(dataset,&dataset_rows,&dataset_columns);

	I_rows=dataset_rows;
	I_columns=dataset_columns-1+1; // no need to do -1+1, this is just for understanding

	I=mlfw_mat_double_create_new(I_rows,I_columns);
	if(I==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		return;
	}

	//copy a=b where is target and b is source
	
	/*
	 1st arg : target matrix
	 2nd arg : source matrix
	 3rd arg : target_row_index
	 4th arg : target_column_index
	 5th arg : source_from_row_index
	 6th arg : source_from__column_index
	 7th arg : source_to_row_index
	 8th arg : source_to_column_index
	 */
	// about 8th argument
	// it means, from 0 to columns-2 are input columns and columns-1 is output column
	// so here we're copying dataset 0th column to last input column 
	// In I matrix 1 index column to last index copying 
	
	mlfw_mat_double_copy(I,dataset,0,1,0,0,dataset_rows-1,dataset_columns-2);

	/*
	 1st arg : matrix to fill
	 2nd arg : from row index
	 3rd arg : from column index
	 4th arg : upto row index
	 5th arg : upto column index
	 6th arg : what to fill
	 */

	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);
	
	m=mlfw_column_vec_double_from_csv(PARAMETERS_FILE_NAME);
	
	if(m==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
		return;
	}

	P=mlfw_multiply_double_matrix_with_column_vector(I,m);
	if(P==NULL)
	{
	
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
		mlfw_column_vec_double_destroy(m);
		return;
	}
	
	// store dataset amd predicted value in result_file
	result_file=fopen(RESULTS_FILE_NAME,"w");
	fprintf(result_file,"input(1),actual_output,predicted_output\n");	

	for(r=0;r<dataset_rows;++r)
	{
		for(c=0;c<dataset_columns;++c)
		{
			fprintf(result_file,"%lf,",mlfw_mat_double_get(dataset,r,c));
		}
		fprintf(result_file,"%lf\n",mlfw_column_vec_double_get(P,r));
	}
	fclose(result_file);
		// release resources
	
                mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_column_vec_double_destroy(m); // the last m which was actually UM
		mlfw_column_vec_double_destroy(P);
}



int main(int argc,char *argv[])
{
	if(argc!=4)
	{
		printf("[Usage : test_it.out dataset_name parameters_file_name result_file_name] \n");
		return 0;
	}
	DATASET_FILE_NAME=argv[1];
	PARAMETERS_FILE_NAME=argv[2];
	RESULTS_FILE_NAME=argv[3];
	test_it();
	printf("Results are stored in %s\n",RESULTS_FILE_NAME);
	return 0;
}
