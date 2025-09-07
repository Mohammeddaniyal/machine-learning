#include<mlfw.h>
#include<stdlib.h>
#include<stdio.h>

uint64_t number_of_training_examples=0;
uint64_t number_of_columns_in_training_examples=0;
mlfw_mat_double *buffer_matrix_1=NULL;
mlfw_mat_double *buffer_matrix_2=NULL;

#define TRAINING_DATASET "test.csv"
#define BUFFER_SIZE 50

void init_buffers()
{
	mlfw_get_csv_dimensions(TRAINING_DATASET,&number_of_training_examples,&number_of_columns_in_training_examples);
	if(mlfw_error()) return;
	if(number_of_training_examples>=BUFFER_SIZE)
	{
		buffer_matrix_1=mlfw_mat_double_create_new(BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	if(number_of_training_examples%BUFFER_SIZE>0)
	{
		buffer_matrix_2=mlfw_mat_double_create_new(number_of_training_examples%BUFFER_SIZE,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
	}
	else
	{
		buffer_matrix_2=mlfw_mat_double_create_new(number_of_training_examples,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
}

void get_one_from_file_buffer(mlfw_mat_double **target_matrix,uint64_t from_row)
{
	// this function will only get called till records are available
	// it won't happen that  the demand is invalid
	
	static uint64_t buffer_matrix_1_starts_at_row=0;
	static uint64_t buffer_matrix_2_starts_at_row=0;
	static uint64_t buffer_matrix_1_ends_at_row=0;
	static uint8_t buffers_populated=0;

	static long f_position=0;

	dimension_t buffer_matrix_1_rows=0;
	dimension_t buffer_matrix_1_columns=0;
	dimension_t buffer_matrix_2_rows=0;
	dimension_t buffer_matrix_2_columns=0;

	uint64_t buffer_matrix_1_last_possible_row;

	index_t from_buffer_matrix_index;
	index_t to_buffer_matrix_index;

	if(buffer_matrix_1!=NULL)
	{
		mlfw_mat_double_get_dimensions(buffer_matrix_1,&buffer_matrix_1_rows,&buffer_matrix_1_columns);
	}
	if(buffer_matrix_2!=NULL)
	{
		mlfw_mat_double_get_dimensions(buffer_matrix_2,&buffer_matrix_2_rows,&buffer_matrix_2_columns);
	}
	if(buffer_matrix_1!=NULL)
	{
		buffer_matrix_1_last_possible_row=number_of_training_examples-(number_of_training_examples%buffer_matrix_1_rows);
	}
	else
	{
		buffer_matrix_1_last_possible_row=0;
	}

	// matrix creation and setting up variables done
	// if target matrix is NULL
	if(*target_matrix==NULL)
	{
		*target_matrix=mlfw_mat_double_create_new(1,number_of_columns_in_training_examples);
		if(mlfw_error()) return;
	}
	if(buffers_populated==0)
	{
		buffers_populated=1;
		if(buffer_matrix_2!=NULL)
		{
			f_position=0;
			mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_2,buffer_matrix_1_last_possible_row+1,buffer_matrix_2_rows,&f_position);
			if(mlfw_error()) return;
			f_position=0;
			buffer_matrix_2_starts_at_row=buffer_matrix_1_last_possible_row+1;
		}
		if(buffer_matrix_1!=NULL)
		{
			f_position=0;
			mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_1,1,buffer_matrix_1_rows,&f_position);
			if(mlfw_error()) return;
			f_position=0;
			buffer_matrix_1_starts_at_row=1;
			buffer_matrix_1_ends_at_row=buffer_matrix_1_starts_at_row+buffer_matrix_1_rows-1;
		}
	}
	if(from_row>buffer_matrix_1_last_possible_row)
	{
		// serve from buffer_matrix_2

		from_buffer_matrix_index=from_row-buffer_matrix_2_starts_at_row;
		to_buffer_matrix_index=from_buffer_matrix_index; // because 1 only
		mlfw_mat_double_copy(*target_matrix,buffer_matrix_2,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
		return;
	}
	if(from_row>=buffer_matrix_1_starts_at_row && from_row<=buffer_matrix_1_ends_at_row)
	{
		// data available in buffer_matrix_1
		from_buffer_matrix_index=from_row-buffer_matrix_1_starts_at_row;
		to_buffer_matrix_index=from_buffer_matrix_index;
		mlfw_mat_double_copy(*target_matrix,buffer_matrix_1,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
		return;
	}

	// buffer matrix 1 reload required
	if(from_row==1)
	{
		f_position=0;
		buffer_matrix_1_starts_at_row=1;
	}
	else
	{
		buffer_matrix_1_starts_at_row=buffer_matrix_1_ends_at_row+1;
	}
	mlfw_mat_double_get_block_from_csv(TRAINING_DATASET,buffer_matrix_1,buffer_matrix_1_starts_at_row,buffer_matrix_1_rows,&f_position);
	buffer_matrix_1_ends_at_row=buffer_matrix_1_starts_at_row+buffer_matrix_1_rows-1;
	
	from_buffer_matrix_index=from_row-buffer_matrix_1_starts_at_row;
	to_buffer_matrix_index=from_buffer_matrix_index;
	mlfw_mat_double_copy(*target_matrix,buffer_matrix_1,0,0,from_buffer_matrix_index,0,to_buffer_matrix_index,number_of_columns_in_training_examples-1);
}
int main()
{
	int x,y;
	char error_string[512];
	dimension_t rows,columns;
	mlfw_mat_double *target_matrix=NULL;
	init_buffers();
	if(mlfw_error()) goto err;
	for(y=1;y<=3;++y)
	{
		for(x=1;x<=number_of_training_examples;++x)
		{
			get_one_from_file_buffer(&target_matrix,x);
			if(mlfw_error()) goto err;
			mlfw_mat_double_get_dimensions(target_matrix,&rows,&columns);
			printf("%d) %lf %lf\n",x,mlfw_mat_double_get(target_matrix,0,0),mlfw_mat_double_get(target_matrix,0,0));
		}
		printf("Total : %d\n",x);
	}
	return 0;
err:
	mlfw_get_error_string(error_string,512);
	printf("Error : %s\n",error_string);
	return 0;
}



