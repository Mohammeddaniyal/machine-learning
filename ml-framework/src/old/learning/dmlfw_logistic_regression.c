#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_set.h>
#include<dmlfw_operations.h>
#include<inttypes.h>
#include<stdio.h>
#include<dmlfw_math.h>
#include<stdlib.h>
dmlfw_row_vec_double * dmlfw_logistic_regression_gradient_descent_fit(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_values_vector,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost))
{
	int error_flag;
	index_t r;
	uint64_t k;

	double sum;
	double cost;

	dmlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	dmlfw_column_vec_double *A;
	dimension_t A_size;

	dmlfw_column_vec_double *m;

	dmlfw_column_vec_double *P;
	dmlfw_column_vec_double *SP;

	dmlfw_column_vec_double *E;


	dmlfw_mat_double *IT;

	dmlfw_column_vec_double *ITE;

	dmlfw_column_vec_double *TMP;

	dmlfw_column_vec_double *UM;

	dmlfw_row_vec_double *trained_parameters;

	dmlfw_column_vec_double *t1;
	dmlfw_column_vec_double *t2;
	dmlfw_column_vec_double *t3;
	dmlfw_column_vec_double *t4;
	dmlfw_column_vec_double *t5;

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
		SP=dmlfw_column_vec_double_create_new(I_rows);
		if(SP==NULL)
		{	
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
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
			dmlfw_column_vec_double_destroy(SP);
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
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			return NULL;
		}
		TMP=dmlfw_column_vec_double_create_new(I_columns);
		if(TMP==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
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
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			return NULL;
		}
		
		t1=dmlfw_column_vec_double_create_new(I_rows);
		if(t1==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			dmlfw_column_vec_double_destroy(UM);
			return NULL;
		}	
		t2=dmlfw_column_vec_double_create_new(I_rows);
		if(t2==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			dmlfw_column_vec_double_destroy(UM);
			dmlfw_column_vec_double_destroy(t1);
			return NULL;
		}	
		t3=dmlfw_column_vec_double_create_new(I_rows);
		if(t3==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			dmlfw_column_vec_double_destroy(UM);
			dmlfw_column_vec_double_destroy(t1);
			dmlfw_column_vec_double_destroy(t2);
			return NULL;
		}	
		t4=dmlfw_column_vec_double_create_new(I_rows);
		if(t4==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			dmlfw_column_vec_double_destroy(UM);
			dmlfw_column_vec_double_destroy(t1);
			dmlfw_column_vec_double_destroy(t2);
			dmlfw_column_vec_double_destroy(t3);
			return NULL;
		}	
		t5=dmlfw_column_vec_double_create_new(I_rows);
		if(t5==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(IT);
			dmlfw_column_vec_double_destroy(m);
			dmlfw_column_vec_double_destroy(P);
			dmlfw_column_vec_double_destroy(SP);
			dmlfw_column_vec_double_destroy(E);
			dmlfw_column_vec_double_destroy(ITE);
			dmlfw_column_vec_double_destroy(TMP);
			dmlfw_column_vec_double_destroy(UM);
			dmlfw_column_vec_double_destroy(t1);
			dmlfw_column_vec_double_destroy(t2);
			dmlfw_column_vec_double_destroy(t3);
			dmlfw_column_vec_double_destroy(t4);
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

		SP=dmlfw_column_vec_double_sigmoid(P,SP);
		if(SP==NULL)
		{
			error_flag=1;
			break;
		}
		// code to calculate cost
		t1=dmlfw_column_vec_double_log(SP,t1);
		if(t1==NULL)
		{
			error_flag=1;
			break;
		}
		t2=dmlfw_element_wise_multiply_double_column_vector(A,t1,t2);
		if(t2==NULL)
		{	
			error_flag=1;
			break;
		}
		t3=dmlfw_subtract_double_column_vector_from_scalar(SP,1,t3);
		if(t3==NULL)
		{
			error_flag=1;
			break;
		}
		t4=dmlfw_column_vec_double_log(t3,t4);		
		if(t4==NULL)
		{
			error_flag=1;
			break;
		}
		t5=dmlfw_subtract_double_column_vector_from_scalar(A,1,t5);
		if(t5==NULL)
		{
			error_flag=1;
			break;
		}
		t3=dmlfw_element_wise_multiply_double_column_vector(t5,t4,t3);
		if(t3==NULL)
		{
			error_flag=1;
			break;
		}
		t1=dmlfw_add_double_column_vector(t2,t3,t1);
		if(t1==NULL)
		{
			error_flag=1;
			break;
		}
		sum=dmlfw_column_vec_double_sum(t1);
		cost=-(sum/(double)I_rows);

		/*
		 a=b-c or z=x-y
		 1st arg : left operand
		 2nd arg : right operand
		 */

		E=dmlfw_subtract_double_column_vector(SP,A,E);
		if(E==NULL)
		{
			error_flag=1;
			break;
		}

	
	
	// logic to update m and c, technically our m column vector

	ITE=dmlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;
	}

	TMP=dmlfw_multiply_double_scalar_with_column_vector((learning_rate*(1.0/I_rows)),ITE,TMP);

	if(TMP==NULL)
	{
		error_flag=1;
		break;
	}

	UM=dmlfw_subtract_double_column_vector(m,TMP,UM);
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
		if(on_each_iteration(k,cost)==0) break;
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
		dmlfw_column_vec_double_destroy(SP);
		dmlfw_column_vec_double_destroy(E);
		dmlfw_column_vec_double_destroy(ITE);
		dmlfw_column_vec_double_destroy(TMP);
		dmlfw_column_vec_double_destroy(UM);
		dmlfw_column_vec_double_destroy(m);
		dmlfw_column_vec_double_destroy(t1);
		dmlfw_column_vec_double_destroy(t2);
		dmlfw_column_vec_double_destroy(t3);
		dmlfw_column_vec_double_destroy(t4);
		dmlfw_column_vec_double_destroy(t5);

		return NULL; 
	}

	// release resources
		
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);

                dmlfw_mat_double_destroy(IT);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(SP);
		dmlfw_column_vec_double_destroy(E);
		dmlfw_column_vec_double_destroy(ITE);
		dmlfw_column_vec_double_destroy(TMP);
		dmlfw_column_vec_double_destroy(UM);
		dmlfw_column_vec_double_destroy(t1);
		dmlfw_column_vec_double_destroy(t2);
		dmlfw_column_vec_double_destroy(t3);
		dmlfw_column_vec_double_destroy(t4);
		dmlfw_column_vec_double_destroy(t5);
		trained_parameters=dmlfw_column_vec_double_transpose(m,NULL);
		dmlfw_column_vec_double_destroy(m);

		return trained_parameters;
}


dmlfw_column_vec_double * dmlfw_logistic_regression_predict(dmlfw_mat_double *input_features_matrix,dmlfw_row_vec_double *trained_parameters)
{
	dimension_t trained_parameters_size;
	dmlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;


	dmlfw_column_vec_double *m;

	dmlfw_column_vec_double *P;
	dmlfw_column_vec_double *SP;


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
	SP=dmlfw_column_vec_double_sigmoid(P,NULL);
	if(SP==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_column_vec_double_destroy(m);
		dmlfw_column_vec_double_destroy(P);
		return NULL;
	}
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_column_vec_double_destroy(m);	
		dmlfw_column_vec_double_destroy(P);
		return SP; // return the column vector with the predicted values
}



dmlfw_mat_double * dmlfw_logistic_regression_gradient_descent_multi_class_fit(dmlfw_mat_double *input_features_matrix,dmlfw_column_vec_double *target_class_vector,dmlfw_set_string *class_set,double learning_rate,uint64_t number_of_iterations,uint8_t (*on_each_iteration) (uint64_t iteration_number,double cost))
{
	dmlfw_mat_double *trained_parameters_matrix;
	dmlfw_row_vec_double *trained_parameters_row_vector;
	dmlfw_column_vec_double *tmp_target_class_vector;
		
	dimension_t trained_parameters_matrix_rows,trained_parameters_matrix_columns;

	dimension_t input_features_matrix_rows,input_features_matrix_columns;
	dimension_t target_class_vector_size;
	dimension_t class_set_size;
	index_t i,j;
	index_t r;
	double target_class_double_value;
	int target_class_int_value;
	int target_class_int_value_from_vector;
	int target_class_int_value_from_set;
	char str[11];
	char *str_ptr;
	double parameter_value;
	if(input_features_matrix==NULL || target_class_vector==NULL || class_set==NULL) return NULL;
	class_set_size=dmlfw_set_string_get_size(class_set);
	if(class_set_size!=0) return NULL;
	dmlfw_mat_double_get_dimensions(input_features_matrix,&input_features_matrix_rows,&input_features_matrix_columns);
	target_class_vector_size=dmlfw_column_vec_double_get_size(target_class_vector);
	if(target_class_vector_size!=input_features_matrix_rows) return NULL;
	for(i=0;i<target_class_vector_size;++i)
	{
		// our idea is to set non fractional values in (set)
		target_class_double_value=dmlfw_column_vec_double_get(target_class_vector,i);
		target_class_int_value=(int)target_class_double_value;
		sprintf(str,"%d",target_class_int_value);
		// problem occured in set add, unable to add due to low memory
		if(dmlfw_set_string_add(class_set,str)==-1) return NULL;
	}
	class_set_size=dmlfw_set_string_get_size(class_set); // this set contains unique values	
	tmp_target_class_vector=dmlfw_column_vec_double_create_new(target_class_vector_size);
	if(tmp_target_class_vector==NULL)
	{
		return NULL;
	}
	
	trained_parameters_matrix_rows=input_features_matrix_columns+1; // +1 because bias will be
									// introduced by the classifier
									// and the trained parameters
									// row that it return will
									// have one extra element
								
	trained_parameters_matrix_columns=class_set_size;

	trained_parameters_matrix=dmlfw_mat_double_create_new(trained_parameters_matrix_rows,trained_parameters_matrix_columns);
	if(trained_parameters_matrix==NULL)
	{
		dmlfw_column_vec_double_destroy(tmp_target_class_vector);
		return NULL;
	}
	// traverse the set and that many times create tmp_target_class_vector and call binary classifier
	for(j=0;j<class_set_size;++j)
	{
		dmlfw_set_string_get(class_set,j,&str_ptr);
		if(str_ptr==NULL)
		{
			dmlfw_column_vec_double_destroy(tmp_target_class_vector);
			dmlfw_mat_double_destroy(trained_parameters_matrix);
			return NULL;
		}
		target_class_int_value_from_set=(int)atoi(str_ptr);
		free(str_ptr);
		for(i=0;i<target_class_vector_size;++i)
		{
			target_class_int_value_from_vector=(int)dmlfw_column_vec_double_get(target_class_vector,i);
			if(target_class_int_value_from_vector==target_class_int_value_from_set)
			{
				dmlfw_column_vec_double_set(tmp_target_class_vector,i,1.0);
			}
			else
			{
				dmlfw_column_vec_double_set(tmp_target_class_vector,i,0.0);
			}
		}
		// we have input features martix and we have tmp target class vector
		// lets call the classifier
		
trained_parameters_row_vector= dmlfw_logistic_regression_gradient_descent_fit(input_features_matrix,tmp_target_class_vector,learning_rate,number_of_iterations,on_each_iteration);

	if(trained_parameters_row_vector==NULL)
	{
		dmlfw_column_vec_double_destroy(tmp_target_class_vector);
		dmlfw_mat_double_destroy(trained_parameters_matrix);	
		return NULL;		
	}
	
	// copy the content of trained_parameters_row_vector into trained_parameters_matrix
	// in the jth column
	for(r=0;r<trained_parameters_matrix_rows;++r)
	{
		parameter_value=dmlfw_row_vec_double_get(trained_parameters_row_vector,r);
		dmlfw_mat_double_set(trained_parameters_matrix,r,j,parameter_value);
	}

	dmlfw_row_vec_double_destroy(trained_parameters_row_vector);	
	
	}// loop on set ends here 
	dmlfw_column_vec_double_destroy(tmp_target_class_vector);
	return trained_parameters_matrix;
}


dmlfw_column_vec_double * dmlfw_logistic_regression_multi_class_predict(dmlfw_mat_double *input_features_matrix,dmlfw_set_string *class_set,dmlfw_mat_double *trained_parameters_matrix)
{
	dimension_t trained_parameters_matrix_rows,trained_parameters_matrix_columns;
	dmlfw_mat_double *I;
	dimension_t I_rows;
	dimension_t I_columns;

	dimension_t class_set_size;


	dmlfw_mat_double *P;
	dmlfw_mat_double *SP;

	index_t r,c;
	double max_value;
	index_t max_index;
	int target_class_int;
	char *str_ptr;
	double value;

	dmlfw_column_vec_double *predicted_target_class_vector;

	if(input_features_matrix==NULL || trained_parameters_matrix==NULL || class_set==NULL) return NULL;
	
	class_set_size=dmlfw_set_string_get_size(class_set);
	if(class_set_size==0) return NULL;
	dmlfw_mat_double_get_dimensions(trained_parameters_matrix,&trained_parameters_matrix_rows,&trained_parameters_matrix_columns);
	

	if(class_set_size!=trained_parameters_matrix_columns) return NULL;

	I=input_features_matrix;

	dmlfw_mat_double_get_dimensions(I,&I_rows,&I_columns);
	if(trained_parameters_matrix_rows!=I_columns+1) return NULL;

	dmlfw_mat_double_reshape(&I,I_rows,I_columns+1);
	if(I==NULL)
	{
		return NULL;
	}
	I_columns=I_columns+1;
	dmlfw_mat_double_right_shift(I,1);
	dmlfw_mat_double_fill(I,0,0,I_rows-1,0,1.0);
	

	P=dmlfw_multiply_double_matrix_with_matrix(I,trained_parameters_matrix,NULL);
	if(P==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		return NULL;
	}
	SP=dmlfw_mat_double_sigmoid(P,NULL);
	if(SP==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_mat_double_destroy(P);
		dmlfw_mat_double_destroy(SP);
		return NULL;
	}
	// determine the target class, by finding the index of max value in each row
	// and then pick the element at that index from the set
	predicted_target_class_vector=dmlfw_column_vec_double_create_new(I_rows);
	if(predicted_target_class_vector==NULL)
	{
		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_mat_double_destroy(P);
		dmlfw_mat_double_destroy(SP);
		return NULL;
	}
	for(r=0;r<I_rows;++r)
	{
		max_value=dmlfw_mat_double_get(SP,r,0);
		max_index=0;
		for(c=1;c<class_set_size;++c)
		{
			value=dmlfw_mat_double_get(SP,r,c);
			if(max_value<value)
			{	
				max_value=value;
				max_index=c;
			}	
		}	
		// now we have the index of max predicted value
		dmlfw_set_string_get(class_set,max_index,&str_ptr);
		if(str_ptr==NULL)
		{
			dmlfw_mat_double_left_shift(I,1);
			dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			dmlfw_mat_double_destroy(P);
			dmlfw_mat_double_destroy(SP);
			dmlfw_column_vec_double_destroy(predicted_target_class_vector);
			return NULL;

		}
		target_class_int=atoi(str_ptr);
		free(str_ptr);
		dmlfw_column_vec_double_set(predicted_target_class_vector,r,(double)target_class_int);
	}

		dmlfw_mat_double_left_shift(I,1);
		dmlfw_mat_double_reshape(&I,I_rows,I_columns-1);
		dmlfw_mat_double_destroy(P);
		dmlfw_mat_double_destroy(SP);
		return predicted_target_class_vector; // return the column vector with the predicted values

}
