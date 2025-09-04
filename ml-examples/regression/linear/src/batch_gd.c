#include<stdio.h>
#include<mlfw.h>
#include<stdlib.h>

#include<___mlfw_error.h>
extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

enum MLFW_GRADIENT_DESCENT_TYPE{MLFW_BATCH_GRADIENT_DESCENT,MLFW_STOCHASTIC_GRADIENT_DESCENT,MLFW_MINI_BATCH_GRADIENT_DESCENT};

typedef struct _mlfw_gradient_descent_options
{
	double learning_rate;
	uint64_t number_of_iterations;
	int (*progress_callback) (uint64_t iteration_number,void *y,void *predicted_y);
	int gradient_descent_type;
	void (*data_provider)(void *x,void *y,uint64_t starting_row_number,uint32_t number_of_rows_to_provide);
	uint32_t mini_batch_size;
}mlfw_gradient_descent_options;

mlfw_gradient_descent_options * mlfw_gradient_descent_options_create_new()
{
	mlfw_gradient_descent_options *gd_options;
	mlfw_reset_error();
	gd_options=(mlfw_gradient_descent_options *)malloc(sizeof(mlfw_gradient_descent_options));
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_LOW_MEMORY_CODE,MLFW_LOW_MEMORY,sizeof(mlfw_gradient_descent_options));
		return NULL;
	}
	gd_options->learning_rate=0.0;
	gd_options->number_of_iterations=0;
	gd_options->progress_callback=NULL;
	gd_options->gradient_descent_type=-1;
	gd_options->data_provider=NULL;
	gd_options->mini_batch_size=0;
	return gd_options;
}
void mlfw_gradient_descent_options_destroy(mlfw_gradient_descent_options *gd_options)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	free(gd_options);
}
void mlfw_gradient_descent_options_set_learning_rate(mlfw_gradient_descent_options *gd_options,double learning_rate)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->learning_rate=learning_rate;
}
void mlfw_gradient_descent_options_set_number_of_iterations(mlfw_gradient_descent_options *gd_options,uint64_t number_of_iterations)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->number_of_iterations=number_of_iterations;
}
void mlfw_gradient_descent_options_set_gradient_descent_type(mlfw_gradient_descent_options *gd_options,int gradient_descent_type)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	if(gradient_descent_type==MLFW_BATCH_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_BATCH_GRADIENT_DESCENT;
	}else if(gradient_descent_type==MLFW_STOCHASTIC_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_STOCHASTIC_GRADIENT_DESCENT;
	}else if(gradient_descent_type==MLFW_MINI_BATCH_GRADIENT_DESCENT)
	{
	gd_options->gradient_descent_type=MLFW_MINI_BATCH_GRADIENT_DESCENT;
	}else
	{
	_mlfw_set_error(MLFW_INVALID_GRADIENT_DESCENT_TYPE_CODE,MLFW_INVALID_GRADIENT_DESCENT_TYPE);
	}
}
void mlfw_gradient_descent_options_set_progress_callback(mlfw_gradient_descent_options *gd_options,int (*progress_callback)(uint64_t,void *,void *))
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->progress_callback=progress_callback;
}
void mlfw_gradient_descent_options_set_data_provider(mlfw_gradient_descent_options *gd_options,void (*data_provider)(void *,void *,uint64_t,uint32_t))
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->data_provider=data_provider;
}
void mlfw_gradient_descent_options_set_mini_batch_size(mlfw_gradient_descent_options *gd_options,uint32_t mini_batch_size)
{
	mlfw_reset_error();
	if(gd_options==NULL)
	{	
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return;
	}
	gd_options->mini_batch_size=mini_batch_size;
}
mlfw_column_vec_double * mlfw_linear_regression_fit_using_batch_gradient_descent(mlfw_gradient_descent_options  *gd_options,mlfw_mat_double *x,mlfw_column_vec_double *y,double regularization_parameter,mlfw_column_vec_double *model)
{
	double learning_rate;
	uint64_t number_of_iterations;

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

	mlfw_column_vec_double *TMP1;
	mlfw_column_vec_double *TMP2;

	mlfw_column_vec_double *gradient_vector;

	mlfw_column_vec_double *UM;


	double bias_parameter_value;
	double sum_of_squared_error_values;
	double regularization_term_value;
	double m_value;
	double m_squared_value;
	double sum_of_squared_parameters_value; // bias is excluded in the sum of square

	double final_error_value;
	double regularized_final_error_value;
	index_t i;
	
	mlfw_reset_error();
	if(gd_options==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"gd_options");
		return NULL;
	}
	if(x==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"x");
		return NULL;
	}
	if(y==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"y");
		return NULL;
	}
	if(gd_options->number_of_iterations==0 && gd_options->progress_callback==NULL)
	{
		// set error
		return NULL;
	}
	
	I=x;
	A=y;
	learning_rate=gd_options->learning_rate;
	number_of_iterations=gd_options->number_of_iterations;
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
		if(model==NULL)
		{
			m=mlfw_column_vec_double_create_new_filled(I_columns,0.0,NULL);
			if(mlfw_error())
			{
				mlfw_mat_double_left_shift(I,1);
				mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
				mlfw_mat_double_destroy(IT);
				return NULL;
			}
		}
		else
		{
			m=model;
		}

// P=I*m (no of rows in I and no of columns in m, which is I_rows*1(because m is column vector)->P, so basically P will be column vector)
		P=mlfw_column_vec_double_create_new(I_rows);
		if(mlfw_error())
		{
			mlfw_mat_double_left_shift(I,1);
			mlfw_mat_double_reshape(&I,I_rows,I_columns-1);
			mlfw_mat_double_destroy(IT);
			mlfw_column_vec_double_destroy(m);
			return NULL;
		}
		// E=P*A column vec * column vec creates another column vec

		E=mlfw_column_vec_double_create_new(I_rows);
		if(mlfw_error())
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
		if(mlfw_error())
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
		if(mlfw_error())
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
		if(mlfw_error())
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
		TMP1=mlfw_column_vec_double_create_new(I_columns);
		if(mlfw_error())
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
		TMP2=mlfw_column_vec_double_create_new(I_columns);
		if(mlfw_error())
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
			mlfw_column_vec_double_destroy(TMP1);
			return NULL;
		}
		gradient_vector=mlfw_column_vec_double_create_new(I_columns);
		if(mlfw_error())
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
			mlfw_column_vec_double_destroy(TMP1);
			mlfw_column_vec_double_destroy(TMP2);
			return NULL;
		}

		UM=mlfw_column_vec_double_create_new(I_columns);
		if(mlfw_error())
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
			mlfw_column_vec_double_destroy(TMP1);
			mlfw_column_vec_double_destroy(TMP2);
			mlfw_column_vec_double_destroy(gradient_vector);
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


		// computing regularization term value
		// computing sum of sqaured value of Q(m vector)
		sum_of_squared_parameters_value=0.0;
		for(i=1;i<I_columns;++i)
		{
			m_value=mlfw_column_vec_double_get(m,i);
			m_squared_value=m_value*m_value;
			sum_of_squared_parameters_value+=m_squared_value;
		}

		final_error_value=sum_of_squared_error_values/(2*I_rows);// reason for dividing by two lec 14 5-6:30 
		
		// computing regularization term
		// (regularization parameter/2*I_rows) * (sum_of_squared_parameters_value)
	regularization_term_value=(regularization_parameter/2*I_rows)*(sum_of_squared_parameters_value);
	regularized_final_error_value=final_error_value+regularization_term_value;
		

	
	
	
	// logic to update m and c, technically our m column vector

	ITE=mlfw_multiply_double_matrix_with_column_vector(IT,E,ITE);
	if(ITE==NULL)
	{
		error_flag=1;
		break;
	}

	TMP1=mlfw_multiply_double_scalar_with_column_vector((1.0/I_rows),ITE,TMP1);

	if(TMP1==NULL)
	{
		error_flag=1;
		break;
	}
	// for ignoring bias
	bias_parameter_value=mlfw_column_vec_double_get(m,0);
	mlfw_column_vec_double_set(m,0,0);

	TMP2=mlfw_multiply_double_scalar_with_column_vector((regularization_parameter)*(1/I_rows),m,TMP2);
	if(TMP2==NULL)
	{
		error_flag=1;
		break;
	}
	mlfw_column_vec_double_set(m,0,bias_parameter_value);
	gradient_vector=mlfw_add_double_column_vector(TMP1,TMP2,gradient_vector);
	if(gradient_vector==NULL)
	{
		error_flag=1;
		break;
	}
	
	TMP1=mlfw_multiply_double_scalar_with_column_vector(learning_rate,gradient_vector,TMP1);
	if(TMP1==NULL)
	{
		error_flag=1;
		break;
	}

	UM=mlfw_subtract_double_column_vector(m,TMP1,UM);
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
	if(gd_options->progress_callback!=NULL)
	{
		if(gd_options->progress_callback(k,A,P)==0) break;
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
		mlfw_column_vec_double_destroy(TMP1);
		mlfw_column_vec_double_destroy(TMP2);
		mlfw_column_vec_double_destroy(gradient_vector);
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
		mlfw_column_vec_double_destroy(TMP1);
		mlfw_column_vec_double_destroy(TMP2);
		mlfw_column_vec_double_destroy(gradient_vector);
		mlfw_column_vec_double_destroy(UM);

		return m;
}
// Everything above this point is being written by Framework Designer
// All the function below this point are being written by Framework User
void load_dataset(mlfw_mat_double **x,mlfw_column_vec_double **y)
{
	mlfw_row_vec_string *header;
	dimension_t rows,columns;
	*x=mlfw_mat_double_from_csv("IceCreamSales.csv",NULL,&header);
	if(mlfw_error()) return;
	mlfw_mat_double_get_dimensions(*x,&rows,&columns);
	if(mlfw_error()) return;
	mlfw_row_vec_string_destroy(header);
	*y=mlfw_mat_double_create_column_vec(*x,columns-1,NULL);
	mlfw_mat_double_reshape(x,rows,columns-1);
}
int on_iteration_complete(uint64_t iteration_number,void *y,void *predicted_y)
{
	printf("Iteration : %" PRIu64 "\n",iteration_number);
	return 1; // keep running
}
mlfw_gradient_descent_options * get_gradient_descent_options()
{
	mlfw_gradient_descent_options *gd_options;
	double learning_rate;
	uint64_t number_of_iterations;
	gd_options=mlfw_gradient_descent_options_create_new();
	if(mlfw_error()) return NULL;
	learning_rate=0.0001;
	number_of_iterations=500000;
	mlfw_gradient_descent_options_set_learning_rate(gd_options,learning_rate);
	mlfw_gradient_descent_options_set_number_of_iterations(gd_options,number_of_iterations);
	mlfw_gradient_descent_options_set_gradient_descent_type(gd_options,MLFW_BATCH_GRADIENT_DESCENT);
	mlfw_gradient_descent_options_set_progress_callback(gd_options,on_iteration_complete);
	return gd_options;
}
int main()
{
	double regularization_parameter;
	char error_string[512];
	char debug_string[512];
	mlfw_column_vec_double *model=NULL;
	mlfw_row_vec_string *model_header=NULL;
	mlfw_mat_double *x=NULL;
	mlfw_column_vec_double *y=NULL;
	mlfw_gradient_descent_options *gd_options=NULL;

	load_dataset(&x,&y);
	regularization_parameter=0.1234;
	if(mlfw_error()) goto err;
	gd_options=get_gradient_descent_options();
	if(mlfw_error()) goto err;
	model=mlfw_linear_regression_fit_using_batch_gradient_descent(gd_options,x,y,regularization_parameter,NULL);
	if(mlfw_error()) goto err;
	model_header=mlfw_row_vec_string_create_new(1);
	if(mlfw_error()) goto err;
	mlfw_row_vec_string_set(model_header,0,"theta");
	if(mlfw_error()) goto err;
	mlfw_column_vec_double_to_csv(model,"example-1-model.csv",model_header);
	if(mlfw_error()) goto err;
	
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	printf("Model saved to examples-1-model.csv\n");
	return 0;
	err:
	mlfw_mat_double_destroy(x);
	mlfw_column_vec_double_destroy(y);
	mlfw_column_vec_double_destroy(model);
	mlfw_row_vec_string_destroy(model_header);
	mlfw_gradient_descent_options_destroy(gd_options);
	mlfw_get_error_string(error_string,512);
	mlfw_get_debug_string(debug_string,512);
	printf("Error : %s\n",error_string);
	printf("Error debug details : %s\n",debug_string);
	return 0;
}
