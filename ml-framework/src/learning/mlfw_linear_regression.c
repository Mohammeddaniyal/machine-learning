#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<inttypes.h>
#include<stdio.h>
mlfw_row_vec_double * mlfw_linear_regression_gradient_descent_fit_line(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double error_value))
{
	int error_flag;
	int x;
	double iteration_number;
	index_t i,j,r;

	uint8_t response;
	uint64_t k;
	dimension_t input_features_matrix_rows;
	dimension_t input_features_matrix_columns;

	dimension_t target_values_vector_size;

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

	if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	if(number_of_iterations<=0) return NULL;
	if(on_each_iteration==NULL) return NULL;
	
	mlfw_mat_double_get_dimensions(input_features_matrix,&input_features_matrix_rows,&input_features_matrix_columns);

	target_values_vector_size=mlfw_column_vec_double_get_size(target_values_vector);
	
	if(input_features_matrix_row!=target_values_vector_size) return NULL;

	A=target_values_vector;
	I=input_features_matrix;
	mlfw_mat_double_reshape(&I,input_features_matrix_rows,input_features_matrix_columns+1);
	mlfw_mat_double_right_shift(I,1);
	I_rows=input_features_matrix_rows;
	I_columns=input_features_matrix_columns+1;
	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);
	



		IT=mlfw_mat_double_transpose(I,NULL);
		if(IT==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			return; 
		}


		
		m=mlfw_column_vec_double_create_new_filled(I_columns,0.0,NULL);
		if(m==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			return;
		}


		// P=I*m (no of rows in I and no of columns in m, which is I_rows*1(because m is column vector)->P, so basically P will be column vector)
		P=mlfw_column_vec_double_create_new(I_rows);
		if(P==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			return;
		}
		// E=P*A column vec * column vec creates another column vec

		E=mlfw_column_vec_double_create_new(I_rows);
		if(E==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			return;
		}
		// ET= E transpose, ET will be a row vector
		
		ET=mlfw_row_vec_double_create_new(I_rows);
		if(ET==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			return;
		}

		// ETE=ET*E, in our case always this is will be size of 1
		ETE=mlfw_column_vec_double_create_new(1);
		if(ETE==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			return;
		}

		// ITE=IT*E, IT_rows*E_columns, since E is columns vector IT_rows*1
		// IT_rows=I_columns
		ITE=mlfw_column_vec_double_create_new(I_columns);
		if(ITE==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			return;
		}
		TMP=mlfw_column_vec_double_create_new(I_columns);
		if(TMP==NULL)
		{
			printf("Low memory\n");
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(A);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			mlfw_column_vec_double_destroy(ITE);
			return;
		}

		UM=mlfw_column_vec_double_create_new(I_columns);
		if(UM==NULL)
		{
			mlfw_mat_double_destroy(I);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(A);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			return;
		}

		error_flag=0;
		// Operations start
		
		k=1;
		
		while(STOP_FLAG==0)
		{	
		if(k==NUMBER_OF_ITERATIONS+1) break;
		P=mlfw_multiply_double_matrix_with_column_vector(I,m,P);
		if(P==NULL)
		{
			error_flag=1;	
			break;
		}

		/*
		 a=b-c or z=x-y
		 1st arg : left operand
		 2nd arg : right operand
		 */

		E=mlfw_subtract_double_column_vector(P,A,E);
		if(E==NULL)
		{
			error_flag=1;
			break;
		}
		ET=mlfw_column_vec_double_transpose(E,ET);
		
		if(ET==NULL)
		{
			error_flag=1;
			break;
		}


		ETE=mlfw_multiply_double_row_vector_with_column_vector(ET,E,ETE);

		if(ETE==NULL)
		{
			error_flag=1;
			break;
		}
		sum_of_squared_error_values=mlfw_column_vec_double_get(ETE,0);
		final_error_value=sum_of_squared_error_values/(2*I_rows);// reason for dividing by two lec 14 5-6:30 

		printf("Iteration Number %" PRIu64 ", Error : %41.15lf\n",k,final_error_value);
		

		iteration_number=(double)k; // type casting uint64_t to double 
	
		response=on_each_iteration(iteration_number,final_error_value);	
		if(response==0) break;
	
	
	// logic to update m and c, technically our m column vector

	ITE=mlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;
	}

	TMP=mlfw_multiply_double_scalar_with_column_vector((LEARNING_RATE*(1.0/I_rows)),ITE,TMP);

	if(TMP==NULL)
	{
		error_flag=1;
		break;
	}

	UM=mlfw_subtract_double_column_vector(m,TMP,UM);
	if(UM==NULL)
	{	
		error_flag=1;
		break;
	}


	// set data from UM to m

	for(r=0;r<I_columns;r++)
	{
		mlfw_column_vec_double_set(m,r,mlfw_column_vec_double_get(UM,r));
	}


	

	++k;
	}

	if(error_flag==1)
	{
		//release all
		printf("Dimension of container is incorrect\n");
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
		mlfw_column_vec_double_destroy(ETE);
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		mlfw_column_vec_double_destroy(UM);
		mlfw_column_vec_double_destroy(m);
		return; 
	}

	// release resources
	
                mlfw_mat_double_destroy(I);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
		mlfw_column_vec_double_destroy(ETE);
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		mlfw_column_vec_double_destroy(UM);
		mlfw_column_vec_double_destroy(m);

		return NULL;
}


