#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<inttypes.h>
#include<stdio.h>
mlfw_row_vec_double * mlfw_linear_regression_gradient_descent_fit_line(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double error_value,mlfw_column_vec_double *predicted_values_vector))
{
	int error_flag;
	index_t r;
	uint64_t k;


	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	mlfw_column_vec_double *A;
	dimension_t A_size;

	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;

	mlfw_column_vec_double *E;

	mlfw_row_vec_double *ET;

	mlfw_column_vec_double *ETE;

	mlfw_mat_double *IT;

	mlfw_column_vec_double *ITE; 

	mlfw_column_vec_double *TMP;

	mlfw_column_vec_double *UM;

	mlfw_row_vec_double *trained_parameters;

	double sum_of_squared_error_values;
	double final_error_value;

	if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	if(number_of_iterations==0 && on_each_iteration==NULL) return NULL;
	
	I=input_features_matrix;
	A=target_values_vector;
	
	mlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	A_size=mlfw_column_vec_double_get_size(A);
	if(I_rows!=A_size)
	{
		return NULL;
	}
	mlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL)
	{
		return NULL;
	}
	I_columns=I_columns+1;
	mlfw_mat_double_right_shift(I,1);

	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0); // fill bias 1.0
	

		IT=mlfw_mat_double_transpose(I,NULL);
		if(IT==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			return NULL; 
		}

		m=mlfw_column_vec_double_create_new_filled(I_columns,0.0,NULL);
		if(m==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			return NULL;
		}


// P=I*m (no of rows in I and no of columns in m, which is I_rows*1(because m is column vector)->P, so basically P will be column vector)
		P=mlfw_column_vec_double_create_new(I_rows);
		if(P==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			return NULL;
		}
		// E=P*A column vec * column vec creates another column vec

		E=mlfw_column_vec_double_create_new(I_rows);
		if(E==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			return NULL;
		}
		// ET= E transpose, ET will be a row vector
		
		ET=mlfw_row_vec_double_create_new(I_rows);
		if(ET==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			return NULL;
		}

		// ETE=ET*E, in our case always this is will be size of 1
		ETE=mlfw_column_vec_double_create_new(1);
		if(ETE==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			return NULL;
		}

		// ITE=IT*E, IT_rows*E_columns, since E is columns vector IT_rows*1
		// IT_rows=I_columns
		ITE=mlfw_column_vec_double_create_new(I_columns);
		if(ITE==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			return NULL;
		}
		TMP=mlfw_column_vec_double_create_new(I_columns);
		if(TMP==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			mlfw_column_vec_double_destroy(ITE);
			return NULL;
		}

		UM=mlfw_column_vec_double_create_new(I_columns);
		if(UM==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(E);
			mlfw_row_vec_double_destroy(ET);
			mlfw_column_vec_double_destroy(ETE);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			return NULL;
		}

		error_flag=0;
		// Operations start
		
		k=1;
		
		while(1)
		{	
		if(number_of_iterations>0 && k==number_of_iterations+1) break;
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

		

	
	
	
	// logic to update m and c, technically our m column vector

	ITE=mlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;
	}

	TMP=mlfw_multiply_double_scalar_with_column_vector((learning_rate*(1.0/I_rows)),ITE,TMP);

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


	// one iteration completed, so if on_each_iteration is not NULL, call the callback
	if(on_each_iteration!=NULL)
	{
		if(on_each_iteration(k,final_error_value)==0) break;
	}
	++k;
	}// operation loop ends

	if(error_flag==1)
	{
		//release all
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
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

	// release resources
	
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(E);
		mlfw_row_vec_double_destroy(ET);
		mlfw_column_vec_double_destroy(ETE);
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		mlfw_column_vec_double_destroy(UM);
		trained_parameters=mlfw_column_vec_double_transpose(m,NULL);
		mlfw_column_vec_double_destroy(m);

		return trained_parameters;
}


mlfw_column_vec_double * mlfw_linear_regression_predict(mlfw_mat_double *input_features_matrix,mlfw_row_vec_double *trained_parameters)
{
	dimension_t trained_parameters_size;
	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;


	if(input_features_matrix==NULL || trained_parameters==NULL) return NULL;
	
	I=input_features_matrix;

	mlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	trained_parameters_size=mlfw_row_vec_double_get_size(trained_parameters);
	if(trained_parameters_size!=I_columns+1) return NULL;

	mlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL)
	{
		return NULL;
	}
	I_columns=I_columns+1;
	mlfw_mat_double_right_shift(I,1);
	mlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);
	

	m=mlfw_row_vec_double_transpose(trained_parameters,NULL);
	if(m==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		return NULL;
	}

	P=mlfw_multiply_double_matrix_with_column_vector(I,m,NULL);
	if(P==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_column_vec_double_destroy(m);
		return NULL;
	}
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_column_vec_double_destroy(m);
		return P; // return the column vector with the predicted values
}



mlfw_row_vec_double * mlfw_linear_regression_normal_equation_fit_line(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector)
{
	mlfw_mat_double *X;
	mlfw_mat_double *XT;
	mlfw_mat_double *XTX;
	mlfw_mat_double *INV_XTX;

	dimension_t X_rows;
	dimension_t X_columns;
	

	mlfw_column_vec_double *Y;
	mlfw_column_vec_double *XTY;
	dimension_t Y_size;

	mlfw_column_vec_double *m;
	mlfw_row_vec_double *trained_parameters;

	if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	
	X=mlfw_mat_double_clone(input_features_matrix,NULL);
	if(X==NULL) return NULL;
	Y=target_values_vector;
	
	mlfw_mat_double_get_dimensions(X,&X_rows,&X_columns);
	Y_size=mlfw_column_vec_double_get_size(Y);
	if(X_rows!=Y_size)
	{
		return NULL;
	}
	mlfw_mat_double_reshape(&X,X_rows,X_columns+1);
	if(X==NULL)
	{
		return NULL;
	}
	X_columns=X_columns+1;
	mlfw_mat_double_right_shift(X,1);

	mlfw_mat_double_fill(X,0,0,X_rows-1,0,1.0); // fill bias 1.0
	
	XT=mlfw_mat_double_transpose(X,NULL);
	if(XT==NULL)
	{
		mlfw_mat_double_destroy(X);
		return NULL; 
	}

	XTX=mlfw_multiply_double_matrix_with_matrix(XT,X,NULL);
	if(XTX==NULL)
	{
		mlfw_mat_double_destroy(X);
		mlfw_mat_double_destroy(XT);
		return NULL;
	}
	INV_XTX=mlfw_mat_double_inverse(XTX,NULL);
	if(INV_XTX==NULL)
	{
		mlfw_mat_double_destroy(X);
		mlfw_mat_double_destroy(XT);
		mlfw_mat_double_destroy(XTX);
		return NULL; 
	}

	XTY=mlfw_multiply_double_matrix_with_column_vector(XT,Y,NULL);
	if(XTY==NULL)
	{
		mlfw_mat_double_destroy(X);
		mlfw_mat_double_destroy(XT);
		mlfw_mat_double_destroy(XTX);
		mlfw_mat_double_destroy(INV_XTX);
		return NULL; 
	}
	m=mlfw_multiply_double_matrix_with_column_vector(INV_XTX,XTY,NULL);
	if(m==NULL)
	{
		mlfw_mat_double_destroy(X);
		mlfw_mat_double_destroy(XT);
		mlfw_mat_double_destroy(XTX);
		mlfw_mat_double_destroy(INV_XTX);
		mlfw_column_vec_double_destroy(XTY);
		return NULL;	
	}
	trained_parameters=mlfw_column_vec_double_transpose(m,NULL);
	if(trained_parameters==NULL)
	{
		mlfw_mat_double_destroy(X);
		mlfw_mat_double_destroy(XT);
		mlfw_mat_double_destroy(XTX);
		mlfw_mat_double_destroy(INV_XTX);
		mlfw_column_vec_double_destroy(XTY);
		mlfw_column_vec_double_destroy(m);
		return NULL;	
	}

	mlfw_mat_double_destroy(X);
	mlfw_mat_double_destroy(XT);
	mlfw_mat_double_destroy(XTX);
	mlfw_mat_double_destroy(INV_XTX);
	mlfw_column_vec_double_destroy(XTY);
	mlfw_column_vec_double_destroy(m);
	
	return trained_parameters;
}
