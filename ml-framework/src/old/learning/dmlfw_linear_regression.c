#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_operations.h>
#include<inttypes.h>
#include<stdio.h>
dmlfw_row_vec_double * dmlfw_linear_regression_gradient_descent_fit_line(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_values_vector,double learning_rate,double regularization_parameter,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double error_value,dmlfw_column_vec_double *predicted_values_vector))
{
	int error_flag;
	index_t r;
	uint64_t k;


	dmlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	dmlfw_column_vec_double *A;
	dimension_t A_size;

	dmlfw_column_vec_double *m;

	dmlfw_column_vec_double *P;

	dmlfw_column_vec_double *E;

	dmlfw_row_vec_double *ET;

	dmlfw_column_vec_double *ETE;

	dmlfw_mat_double *IT;

	dmlfw_column_vec_double *ITE; 

	dmlfw_column_vec_double *TMP1;
	dmlfw_column_vec_double *TMP2;

	dmlfw_column_vec_double *gradient_vector;

	dmlfw_column_vec_double *UM;

	dmlfw_row_vec_double *trained_parameters;

	double bias_parameter_value;
	double sum_of_squared_error_values;
	double regularization_term_value;
	double m_value;
	double m_squared_value;
	double sum_of_squared_parameters_value; // bias is excluded in the sum of square

	double final_error_value;
	double regularized_final_error_value;
	index_t i;

	if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	if(number_of_iterations==0 && on_each_iteration==NULL) return NULL;
	
	I=input_features_matrix;
	A=target_values_vector;
	
	dmlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	A_size=dmlfw_column_vec_double_get_size(A);
	if(I_rows!=A_size)
	{
		return NULL;
	}
	dmlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL)
	{
		return NULL;
	}
	I_columns=I_columns+1;
	dmlfw_mat_double_right_shift(I,1);

	dmlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0); // fill bias 1.0
	

		IT=dmlfw_mat_double_transpose(I,NULL);
		if(IT==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			return NULL; 
		}

		m=dmlfw_column_vec_double_create_new_filled(I_columns,0.0,NULL);
		if(m==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			return NULL;
		}


// P=I*m (no of rows in I and no of columns in m, which is I_rows*1(because m is column vector)->P, so basically P will be column vector)
		P=dmlfw_column_vec_double_create_new(I_rows);
		if(P==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			return NULL;
		}
		// E=P*A column vec * column vec creates another column vec

		E=dmlfw_column_vec_double_create_new(I_rows);
		if(E==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			return NULL;
		}
		// ET= E transpose, ET will be a row vector
		
		ET=dmlfw_row_vec_double_create_new(I_rows);
		if(ET==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			return NULL;
		}

		// ETE=ET*E, in our case always this is will be size of 1
		ETE=dmlfw_column_vec_double_create_new(1);
		if(ETE==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			return NULL;
		}

		// ITE=IT*E, IT_rows*E_columns, since E is columns vector IT_rows*1
		// IT_rows=I_columns
		ITE=dmlfw_column_vec_double_create_new(I_columns);
		if(ITE==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			dmlfw_column_vec_double_destroy(ETE);
			return NULL;
		}
		TMP1=dmlfw_column_vec_double_create_new(I_columns);
		if(TMP1==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			dmlfw_column_vec_double_destroy(ETE);
			dmlfw_column_vec_double_destroy(ITE);
			return NULL;
		}
		TMP2=dmlfw_column_vec_double_create_new(I_columns);
		if(TMP2==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			dmlfw_column_vec_double_destroy(ETE);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP1);
			return NULL;
		}
		gradient_vector=dmlfw_column_vec_double_create_new(I_columns);
		if(gradient_vector==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			dmlfw_column_vec_double_destroy(ETE);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP1);
			dmlfw_column_vec_double_destroy(TMP2);
			return NULL;
		}

		UM=dmlfw_column_vec_double_create_new(I_columns);
		if(UM==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_row_vec_double_destroy(ET);
			dmlfw_column_vec_double_destroy(ETE);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP1);
			dmlfw_column_vec_double_destroy(TMP2);
			dmlfw_column_vec_double_destroy(gradient_vector);
			return NULL;
		}

		error_flag=0;
		// Operations start
		
		k=1;
		
		while(1)
		{	
		if(number_of_iterations>0 && k==number_of_iterations+1) break;
		P=dmlfw_multiply_double_matrix_with_column_vector(I,m,P);
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

		E=dmlfw_subtract_double_column_vector(P,A,E);
		if(E==NULL)
		{
			error_flag=1;
			break;
		}
		ET=dmlfw_column_vec_double_transpose(E,ET);
		
		if(ET==NULL)
		{
			error_flag=1;
			break;
		}


		ETE=dmlfw_multiply_double_row_vector_with_column_vector(ET,E,ETE);

		if(ETE==NULL)
		{
			error_flag=1;
			break;
		}
		sum_of_squared_error_values=dmlfw_column_vec_double_get(ETE,0);


		// computing regularization term value
		// computing sum of sqaured value of Q(m vector)
		sum_of_squared_parameters_value=0.0;
		for(i=1;i<I_columns;++i)
		{
			m_value=dmlfw_column_vec_double_get(m,i);
			m_squared_value=m_value*m_value;
			sum_of_squared_parameters_value+=m_squared_value;
		}

		final_error_value=sum_of_squared_error_values/(2*I_rows);// reason for dividing by two lec 14 5-6:30 
		
		// computing regularization term
		// (regularization parameter/2*I_rows) * (sum_of_squared_parameters_value)
	regularization_term_value=(regularization_parameter/2*I_rows)*(sum_of_squared_parameters_value);
	regularized_final_error_value=final_error_value+regularization_term_value;
		

	
	
	
	// logic to update m and c, technically our m column vector

	ITE=dmlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;
	}

	TMP1=dmlfw_multiply_double_scalar_with_column_vector((1.0/I_rows),ITE,TMP1);

	if(TMP1==NULL)
	{
		error_flag=1;
		break;
	}
	// for ignoring bias
	bias_parameter_value=dmlfw_column_vec_double_get(m,0);
	dmlfw_column_vec_double_set(m,0,0);

	TMP2=dmlfw_multiply_double_scalar_with_column_vector((regularization_parameter)*(1/I_rows),m,TMP2);
	if(TMP2==NULL)
	{
		error_flag=1;
		break;
	}
	dmlfw_column_vec_double_set(m,0,bias_parameter_value);
	gradient_vector=dmlfw_add_double_column_vector(TMP1,TMP2,gradient_vector);
	if(gradient_vector==NULL)
	{
		error_flag=1;
		break;
	}
	
	TMP1=dmlfw_multiply_double_scalar_with_column_vector(learning_rate,gradient_vector,TMP1);
	if(TMP1==NULL)
	{
		error_flag=1;
		break;
	}

	UM=dmlfw_subtract_double_column_vector(m,TMP1,UM);
	if(UM==NULL)
	{	
		error_flag=1;
		break;
	}


	// set data from UM to m

	for(r=0;r<I_columns;r++)
	{
		dmlfw_column_vec_double_set(m,r,dmlfw_column_vec_double_get(UM,r));
	}


	// one iteration completed, so if on_each_iteration is not NULL, call the callback
	if(on_each_iteration!=NULL)
	{
		if(on_each_iteration(k,regularized_final_error_value,P)==0) break;
	}
	++k;
	}// operation loop ends

	if(error_flag==1)
	{
		//release all
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                dmlfw_mat_double_destroy(IT);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(E);
		dmlfw_row_vec_double_destroy(ET);
		dmlfw_column_vec_double_destroy(ETE);
		dmlfw_column_vec_double_destroy(ITE);
		dmlfw_column_vec_double_destroy(TMP1);
		dmlfw_column_vec_double_destroy(TMP2);
		dmlfw_column_vec_double_destroy(gradient_vector);
		dmlfw_column_vec_double_destroy(UM);
		dmlfw_column_vec_double_destroy(m);
		return NULL; 
	}

	// release resources
	
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
                dmlfw_mat_double_destroy(IT);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(E);
		dmlfw_row_vec_double_destroy(ET);
		dmlfw_column_vec_double_destroy(ETE);
		dmlfw_column_vec_double_destroy(ITE);
		dmlfw_column_vec_double_destroy(TMP1);
		dmlfw_column_vec_double_destroy(TMP2);
		dmlfw_column_vec_double_destroy(gradient_vector);
		dmlfw_column_vec_double_destroy(UM);
		trained_parameters=dmlfw_column_vec_double_transpose(m,NULL);
		dmlfw_column_vec_double_destroy(m);

		return trained_parameters;
}


dmlfw_column_vec_double * dmlfw_linear_regression_predict(dmlfw_mat_double *input_features_matrix,dmlfw_row_vec_double *trained_parameters)
{
	dimension_t trained_parameters_size;
	dmlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	dmlfw_column_vec_double *m;

	dmlfw_column_vec_double *P;


	if(input_features_matrix==NULL || trained_parameters==NULL) return NULL;
	
	I=input_features_matrix;

	dmlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	trained_parameters_size=dmlfw_row_vec_double_get_size(trained_parameters);
	if(trained_parameters_size!=I_columns+1) return NULL;

	dmlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL)
	{
		return NULL;
	}
	I_columns=I_columns+1;
	dmlfw_mat_double_right_shift(I,1);
	dmlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);
	

	m=dmlfw_row_vec_double_transpose(trained_parameters,NULL);
	if(m==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		return NULL;
	}

	P=dmlfw_multiply_double_matrix_with_column_vector(I,m,NULL);
	if(P==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_column_vec_double_destroy(m);
		return NULL;
	}
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_column_vec_double_destroy(m);
		return P; // return the column vector with the predicted values
}



dmlfw_row_vec_double * dmlfw_linear_regression_normal_equation_fit_line(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_values_vector)
{
	dmlfw_mat_double *X;
	dmlfw_mat_double *XT;
	dmlfw_mat_double *XTX;
	dmlfw_mat_double *INV_XTX;

	dimension_t X_rows;
	dimension_t X_columns;
	

	dmlfw_column_vec_double *Y;
	dmlfw_column_vec_double *XTY;
	dimension_t Y_size;

	dmlfw_column_vec_double *m;
	dmlfw_row_vec_double *trained_parameters;

	if(input_features_matrix==NULL || target_values_vector==NULL) return NULL;
	
	X=dmlfw_mat_double_clone(input_features_matrix,NULL);
	if(X==NULL) return NULL;
	Y=target_values_vector;
	
	dmlfw_mat_double_get_dimensions(X,&X_rows,&X_columns);
	Y_size=dmlfw_column_vec_double_get_size(Y);
	if(X_rows!=Y_size)
	{
		return NULL;
	}
	dmlfw_mat_double_reshape(&X,X_rows,X_columns+1);
	if(X==NULL)
	{
		return NULL;
	}
	X_columns=X_columns+1;
	dmlfw_mat_double_right_shift(X,1);

	dmlfw_mat_double_fill(X,0,0,X_rows-1,0,1.0); // fill bias 1.0
	
	XT=dmlfw_mat_double_transpose(X,NULL);
	if(XT==NULL)
	{
		dmlfw_mat_double_destroy(X);
		return NULL; 
	}

	XTX=dmlfw_multiply_double_matrix_with_matrix(XT,X,NULL);
	if(XTX==NULL)
	{
		dmlfw_mat_double_destroy(X);
		dmlfw_mat_double_destroy(XT);
		return NULL;
	}
	INV_XTX=dmlfw_mat_double_inverse(XTX,NULL);
	if(INV_XTX==NULL)
	{
		dmlfw_mat_double_destroy(X);
		dmlfw_mat_double_destroy(XT);
		dmlfw_mat_double_destroy(XTX);
		return NULL; 
	}

	XTY=dmlfw_multiply_double_matrix_with_column_vector(XT,Y,NULL);
	if(XTY==NULL)
	{
		dmlfw_mat_double_destroy(X);
		dmlfw_mat_double_destroy(XT);
		dmlfw_mat_double_destroy(XTX);
		dmlfw_mat_double_destroy(INV_XTX);
		return NULL; 
	}
	m=dmlfw_multiply_double_matrix_with_column_vector(INV_XTX,XTY,NULL);
	if(m==NULL)
	{
		dmlfw_mat_double_destroy(X);
		dmlfw_mat_double_destroy(XT);
		dmlfw_mat_double_destroy(XTX);
		dmlfw_mat_double_destroy(INV_XTX);
		dmlfw_column_vec_double_destroy(XTY);
		return NULL;	
	}
	trained_parameters=dmlfw_column_vec_double_transpose(m,NULL);
	if(trained_parameters==NULL)
	{
		dmlfw_mat_double_destroy(X);
		dmlfw_mat_double_destroy(XT);
		dmlfw_mat_double_destroy(XTX);
		dmlfw_mat_double_destroy(INV_XTX);
		dmlfw_column_vec_double_destroy(XTY);
		dmlfw_column_vec_double_destroy(m);
		return NULL;	
	}

	dmlfw_mat_double_destroy(X);
	dmlfw_mat_double_destroy(XT);
	dmlfw_mat_double_destroy(XTX);
	dmlfw_mat_double_destroy(INV_XTX);
	dmlfw_column_vec_double_destroy(XTY);
	dmlfw_column_vec_double_destroy(m);
	
	return trained_parameters;
}
