#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<inttypes.h>
#include<stdio.h>
#include<mlfw_math.h>
mlfw_row_vec_double * mlfw_logistic_regression_gradient_descent_fit(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost))
{
	int error_flag;
	index_t r;
	uint64_t k;

	double sum;
	double cost;

	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	mlfw_column_vec_double *A;
	dimension_t A_size;

	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;
	mlfw_column_vec_double *SP;

	mlfw_column_vec_double *E;


	mlfw_mat_double *IT;

	mlfw_column_vec_double *ITE;

	mlfw_column_vec_double *TMP;

	mlfw_column_vec_double *UM;

	mlfw_row_vec_double *trained_parameters;

	mlfw_column_vec_double *t1;
	mlfw_column_vec_double *t2;
	mlfw_column_vec_double *t3;
	mlfw_column_vec_double *t4;
	mlfw_column_vec_double *t5;

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
		SP=mlfw_column_vec_double_create_new(I_rows);
		if(SP==NULL)
		{	
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
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
			mlfw_column_vec_double_destroy(SP);
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
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
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
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
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
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			return NULL;
		}
		
		t1=mlfw_column_vec_double_create_new(I_rows);
		if(t1==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			mlfw_column_vec_double_destroy(UM);
			return NULL;
		}	
		t2=mlfw_column_vec_double_create_new(I_rows);
		if(t2==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			mlfw_column_vec_double_destroy(UM);
			mlfw_column_vec_double_destroy(t1);
			return NULL;
		}	
		t3=mlfw_column_vec_double_create_new(I_rows);
		if(t3==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			mlfw_column_vec_double_destroy(UM);
			mlfw_column_vec_double_destroy(t1);
			mlfw_column_vec_double_destroy(t2);
			return NULL;
		}	
		t4=mlfw_column_vec_double_create_new(I_rows);
		if(t4==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			mlfw_column_vec_double_destroy(UM);
			mlfw_column_vec_double_destroy(t1);
			mlfw_column_vec_double_destroy(t2);
			mlfw_column_vec_double_destroy(t3);
			return NULL;
		}	
		t5=mlfw_column_vec_double_create_new(I_rows);
		if(t5==NULL)
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			mlfw_column_vec_double_destroy(P);
			mlfw_column_vec_double_destroy(SP);
			mlfw_column_vec_double_destroy(E);
			mlfw_column_vec_double_destroy(ITE);
			mlfw_column_vec_double_destroy(TMP);
			mlfw_column_vec_double_destroy(UM);
			mlfw_column_vec_double_destroy(t1);
			mlfw_column_vec_double_destroy(t2);
			mlfw_column_vec_double_destroy(t3);
			mlfw_column_vec_double_destroy(t4);
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

		SP=mlfw_column_vec_double_sigmoid(P,SP);
		if(SP==NULL)
		{
			error_flag=1;
			break;
		}
		// code to calculate cost
		t1=mlfw_column_vec_double_log(SP,t1);
		if(t1==NULL)
		{
			error_flag=1;
			break;
		}
		t2=mlfw_element_wise_multiply_double_column_vector(A,t1,t2);
		if(t2==NULL)
		{	
			error_flag=1;
			break;
		}
		t3=mlfw_subtract_double_column_vector_from_scalar(SP,1,t3);
		if(t3==NULL)
		{
			error_flag=1;
			break;
		}
		t4=mlfw_column_vec_double_log(t3,t4);		
		if(t4==NULL)
		{
			error_flag=1;
			break;
		}
		t5=mlfw_subtract_double_column_vector_from_scalar(A,1,t5);
		if(t5==NULL)
		{
			error_flag=1;
			break;
		}
		t3=mlfw_element_wise_multiply_double_column_vector(t5,t4,t3);
		if(t3==NULL)
		{
			error_flag=1;
			break;
		}
		t1=mlfw_add_double_column_vector(t2,t3,t1);
		if(t1==NULL)
		{
			error_flag=1;
			break;
		}
		sum=mlfw_column_vec_double_sum(t1);
		cost=-(sum/(double)I_rows);

		/*
		 a=b-c or z=x-y
		 1st arg : left operand
		 2nd arg : right operand
		 */

		E=mlfw_subtract_double_column_vector(SP,A,E);
		if(E==NULL)
		{
			error_flag=1;
			break;
		}

	
	
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
		if(on_each_iteration(k,cost)==0) break;
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
		mlfw_column_vec_double_destroy(SP);
		mlfw_column_vec_double_destroy(E);
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		mlfw_column_vec_double_destroy(UM);
		mlfw_column_vec_double_destroy(m);
		mlfw_column_vec_double_destroy(t1);
		mlfw_column_vec_double_destroy(t2);
		mlfw_column_vec_double_destroy(t3);
		mlfw_column_vec_double_destroy(t4);
		mlfw_column_vec_double_destroy(t5);

		return NULL; 
	}

	// release resources
		
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);

                mlfw_mat_double_destroy(IT);
		mlfw_column_vec_double_destroy(P);
		mlfw_column_vec_double_destroy(SP);
		mlfw_column_vec_double_destroy(E);
		mlfw_column_vec_double_destroy(ITE);
		mlfw_column_vec_double_destroy(TMP);
		mlfw_column_vec_double_destroy(UM);
		mlfw_column_vec_double_destroy(t1);
		mlfw_column_vec_double_destroy(t2);
		mlfw_column_vec_double_destroy(t3);
		mlfw_column_vec_double_destroy(t4);
		mlfw_column_vec_double_destroy(t5);
		trained_parameters=mlfw_column_vec_double_transpose(m,NULL);
		mlfw_column_vec_double_destroy(m);

		return trained_parameters;
}


mlfw_column_vec_double * mlfw_logistic_regression_predict(mlfw_mat_double *input_features_matrix,mlfw_row_vec_double *trained_parameters)
{
	dimension_t trained_parameters_size;
	mlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	mlfw_column_vec_double *m;

	mlfw_column_vec_double *P;
	mlfw_column_vec_double *SP;


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
	SP=mlfw_column_vec_double_sigmoid(P,NULL);
	if(SP==NULL)
	{
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_column_vec_double_destroy(m);
		mlfw_column_vec_double_destroy(P);
		return NULL;
	}
		mlfw_mat_double_left_shift(I,1);
		mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		mlfw_column_vec_double_destroy(m);	
		mlfw_column_vec_double_destroy(P);
		return SP; // return the column vector with the predicted values
}



mlfw_mat_double * mlfw_logistic_regression_gradient_descent_multi_class_fit(mlfw_mat_double *input_features_matrix,mlfw_column_vec_double *target_class_vector,mlfw_set_string *class_set,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost))
{
	mlfw_mat_double *trained_parameters_matrix;
	mlfw_row_vec_double *trained_parameters_row_vector;
	mlfw_column_vec_double *tmp_target_class_vector;

	dimension_t input_features_matrix_rows,input_features_matrix_columns;
	dimension_t target_class_vector_size;
	dimension_t class_set_size;
	index_t i,j;
	double target_class_double_value;
	int target_class_int_value;
	int target_class_int_value_from_set;
	char str[11];
	char *str_ptr;
	if(input_features_matrix==NULL || target_class_vector==NULL || class_set==NULL) return NULL;
	class_set_size=mlfw_set_string_get_size(class_set);
	if(class_set_size!=0) return NULL;
	mlfw_mat_double_get_dimensions(input_features_matrix,&input_features_matrix_rows,&input_features_matrix_columns);
	target_class_vector_size=mlfw_column_vec_double_get_size(target_class_vector);
	if(target_class_vector_size!=input_features_matrix_rows) return NULL;
	for(i=0;i<target_class_vector_size;++i)
	{
		// our idea is to set non fractional values in (set)
		target_class_double_value=mlfw_column_vec_double_get(target_class_vector,i);
		target_class_int_value=(int)target_class_double_value;
		sprintf(str,"%d",target_class_int_value);
		mlfw_set_string_add(class_set,str);
	}
	
	tmp_target_class_vector=mlfw_column_vec_double_create_new(target_class_vector_size);
	if(tmp_target_class_vector==NULL)
	{
		return NULL;
	}

	for(j=0;j<class_set_size;++j)
	{
		mlfw_set_string_get(class_set,j,&str_ptr);
		if(str_ptr==NULL)
		{
			mlfw_column_vec_double_destroy(tmp_target_class_vector);
			return NULL;
		}
		target_class_int_value_from_set=(int)atoi(str_ptr);
		free(str_ptr);
		for(i=0;i<target_class_vector_size;++i)
		{
			target_class_int_value=mlfw_column_vec_double_get(target_class_vector,i);
			if(target_class_int_value==target_class_int_value_from_set)
			{
				mlfw_column_vec_double_set(tmp_target_class_vector,i,1.0);
			}
			else
			{
				mlfw_column_vec_double_set(tmp_target_class_vector,i,0.0);
			}
		}

	}

	

	return trained_parameters_matrix;
}
