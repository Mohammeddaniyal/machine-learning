#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>
// global variable
uint64_t NUMBER_OF_ITERATIONS=-1;
char *DATASET_FILE_NAME=NULL;
double LEARNING_RATE=1.0;
dimension_t HISTORY_SIZE=0;
char *HISTORY_FILE_NAME=NULL;
char *PARAMETERS_FILE_NAME=NULL;
char *GRAPH_FILE_NAME=NULL;



uint8_t STOP_FLAG=0;

void train_it()
{
	FILE *graph_file;
	mlfw_mat_double *history;
	dimension_t history_rows,history_columns;
	index_t history_index;
	double iteration_number;
	index_t i,j;

	uint64_t k;
	mlfw_mat_double *dataset;
	dimension_t dataset_rows;
	dimension_t dataset_columns;
	
	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;

	mlfw_column_vec_double *A;

	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;

	mlfw_column_vec_double *E;

	mlfw_row_vec_double *ET;

	mlfw_column_vec_double *ETE;

	mlfw_mat_double *IT;

	mlfw_column_vec_double *ITE; 

	mlfw_column_vec_double *TMP;

	mlfw_column_vec_double *UM;

	double sum_of_squared_error_values;
	double final_error_value;

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

	IT=mlfw_mat_double_transpose(I);
	if(IT==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
		return; 
	}

	/*
	1st arg : source matrix
	2nd arg : which column to use to create column vector
	 */

	A=mlfw_mat_double_create_column_vec(dataset,dataset_columns-1); // last column is the output feature
	if(A==NULL)
	{
		printf("Unable to create column vector\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		return;
	}
	// We had discussed that c will be 0 and m will be 1
	// But we had also discussed that value of m and c can be anything
	// hence we're taking c as 0 and m also as 0
	
	m=mlfw_column_vec_double_create_new_filled(I_columns,0.0);
	if(m==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(A);
		return;
	}

	history_rows=HISTORY_SIZE;
	history_columns=I_columns+2; // 1 extra for iteration_number 
				     // 1 extra for error value at 0 index
	history=mlfw_mat_double_create_new(history_rows,history_columns);
	if(history==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(A);
		mlfw_column_vec_double_destroy(m);
		return;
	}

	// reason lec 15 56:10

	// filling with initial value as 0.0 to avoid garbage value in whole history matrix

	mlfw_mat_double_fill(history,0,0,history_rows-1,history_columns-1,0.0);

	history_index=0;

	graph_file=fopen(GRAPH_FILE_NAME,"w");

	// Operations start
	
	k=1;
	
	while(STOP_FLAG==0)
	{	
	if(k==NUMBER_OF_ITERATIONS+1) break;
	P=mlfw_multiply_double_matrix_with_column_vector(I,m);
	if(P==NULL)
	{
	
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
		mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(A);
		mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
		return;
	}

	/*
	 a=b-c or z=x-y
	 1st arg : left operand
	 2nd arg : right operand
	 */

	E=mlfw_subtract_double_column_vector(P,A);
	if(E==NULL)
	{
       		printf("Low memory\n");
                mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
        	mlfw_column_vec_double_destroy(P);
		return;
	}
	ET=mlfw_column_vec_double_transpose(E);
	
	if(ET==NULL)
	{
	    	mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
                mlfw_column_vec_double_destroy(P);
                mlfw_column_vec_double_destroy(E);
		return;
	}


	ETE=mlfw_multiply_double_row_vector_with_column_vector(ET,E);

	if(ETE==NULL)
	{
		printf("Low memory\n");
	    	mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
                mlfw_column_vec_double_destroy(P);
                mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
		return;
	}
	sum_of_squared_error_values=mlfw_column_vec_double_get(ETE,0);
	// reason for using 2
	// we introduced 2 for the derivative to make the expression simple
	// in lec 13 
	final_error_value=sum_of_squared_error_values/(2*I_rows);// reason for dividing by two lec 14 5-6:30 

	//printf("Sum of squared error values : %41.15lf\n",sum_of_squared_error_values);
	printf("Iteration Number %" PRIu64 ", Error : %41.15lf\n",k,final_error_value);
	
	fprintf(graph_file,"%" PRIu64 ",%lf\n",k,final_error_value);

	iteration_number=(double)k; // type casting uint64_t to double 
	
	// logic to add history starts here
	if(history_index==HISTORY_SIZE)
	{
		for(i=1;i<HISTORY_SIZE;++i)
		{
			for(j=0;j<history_columns;++j)
			{
				mlfw_mat_double_set(history,i-1,j,mlfw_mat_double_get(history,i,j));
			}
		}
		history_index--;
	}
	
	mlfw_mat_double_set(history,history_index,0,iteration_number);
	mlfw_mat_double_set(history,history_index,1,final_error_value);
	j=mlfw_column_vec_double_get_size(m);
	for(i=0;i<j;++i)
	{
		mlfw_mat_double_set(history,history_index,2+i,mlfw_column_vec_double_get(m,i));
	}
	history_index++;
	// logic to add history ends here

	// logic to update m and c, technically our m column vector

	ITE=mlfw_multiply_double_matrix_with_column_vector(IT,E);
	if(ITE==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
	       	mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
                mlfw_column_vec_double_destroy(P);
                mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
                mlfw_column_vec_double_destroy(ETE);	
		return;
	}

	TMP=mlfw_multiply_double_scalar_with_column_vector((LEARNING_RATE*(1.0/I_rows)),ITE);

	if(TMP==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
	       	mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
                mlfw_column_vec_double_destroy(P);
                mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
                mlfw_column_vec_double_destroy(ETE);	
		mlfw_column_vec_double_destroy(ITE);
		return;
	}

	UM=mlfw_subtract_double_column_vector(m,TMP);
	if(UM==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
	       	mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m);
		mlfw_mat_double_destroy(history);
                mlfw_column_vec_double_destroy(P);
                mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
                mlfw_column_vec_double_destroy(ETE);	
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		return;
	}

	mlfw_column_vec_double_destroy(m); // Old m vector release
	m=UM; // now m is pointing to newly create vector with updated values


	mlfw_column_vec_double_destroy(P);
	mlfw_column_vec_double_destroy(E);
	mlfw_row_vec_double_destroy(ET);
	mlfw_column_vec_double_destroy(ETE);
	mlfw_column_vec_double_destroy(ITE);
	mlfw_column_vec_double_destroy(TMP);

	++k;
	}

	fclose(graph_file);
	// code to store the contents of  (m vector) to csv file
	
		mlfw_mat_double_to_csv(history,HISTORY_FILE_NAME);
		mlfw_column_vec_double_to_csv(m,PARAMETERS_FILE_NAME);	
	// release resources
	
                mlfw_mat_double_destroy(dataset);
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
                mlfw_column_vec_double_destroy(A);
                mlfw_column_vec_double_destroy(m); // the last m which was actually UM
		mlfw_mat_double_destroy(history);
}

// lec 15 17:00
void * thread_function(void *p)
{
	train_it(); // train_it will get executed on thread on which thread function
		    // is being executed
	return NULL;
}

int main(int argc,char *argv[])
{
	pthread_t thread_id;
	int result;
	char m;
	char *ptr;
	if(argc!=7 && argc!=8)
	{
		printf("[Usage : train_it.out dataset_name learning_rate history_size histroy_file_name parameters_file_name graph_data_file_name number_of_iterations(optional)] \n");
	return 0;
	}
	DATASET_FILE_NAME=argv[1];
	ptr=NULL; // reason lec 15 module 1 (A15) 9:00  
	LEARNING_RATE=strtod(argv[2],&ptr);
	ptr=NULL;
	HISTORY_SIZE=(dimension_t)strtoull(argv[3],&ptr,10);
	if(HISTORY_SIZE<5)
	{
		printf("History size should be greater then 5\n");
		return 0;
	}
	HISTORY_FILE_NAME=argv[4];
	PARAMETERS_FILE_NAME=argv[5];
	GRAPH_FILE_NAME=argv[6];
	if(argc==8)
	{
		ptr=NULL;
		// ull means unsigned long log
		// passing 10, because string is in decimal form
		NUMBER_OF_ITERATIONS=strtoull(argv[7],&ptr,10);
	}
	
	result=pthread_create(&thread_id,NULL,thread_function,NULL);
	if(result!=0)
	{
		printf("Unable to create thread\n");
		return 0;
	}
	while(1)
	{
		m=getchar();
		if(m=='\n')
		{
			STOP_FLAG=1;
			break;
		}
	}
	pthread_join(thread_id,NULL); // wait for the thread function to end
	return 0;
}
