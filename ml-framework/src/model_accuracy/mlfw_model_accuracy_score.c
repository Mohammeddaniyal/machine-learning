#include<mlfw_model_accuracy.h>
#include<mlfw_vector.h>
#include<mlfw_matrix.h>
#include<mlfw_operations.h>
#include<stdio.h>
#include<mlfw_error.h>
#include<___mlfw_error.h>

extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

double mlfw_get_r2_score(mlfw_column_vec_double *target_values_vector,mlfw_column_vec_double *predicted_values_vector)
{
	dimension_t target_values_vector_size;
	dimension_t predicted_values_vector_size;
	mlfw_column_vec_double *A;
	mlfw_column_vec_double *P;
	mlfw_column_vec_double *R; //  A-P
	mlfw_row_vec_double *RT; // transpose of R
	mlfw_column_vec_double *RTR; // RT*R
	double mean_of_actuals;
	mlfw_column_vec_double *M;
	mlfw_column_vec_double *AM; // A-M
	mlfw_row_vec_double *AMT; // transpose of AM
	mlfw_column_vec_double *AMTAM; // AMT*AM
	
	double SSR;
	double SST;
	double r2score;

	mlfw_reset_error();

    if (target_values_vector == NULL) {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "target_values_vector");
        return 0.0;
    }
    if (predicted_values_vector == NULL) {
        _mlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "predicted_values_vector");
        return 0.0;
    }
	target_values_vector_size=mlfw_column_vec_double_get_size(target_values_vector);
	predicted_values_vector_size=mlfw_column_vec_double_get_size(predicted_values_vector);
	if(target_values_vector_size!=predicted_values_vector_size)
	{
		 _mlfw_set_error(MLFW_VECTOR_SIZE_NOT_SAME_CODE, MLFW_VECTOR_SIZE_NOT_SAME,"target_values_vector", "predicted_values_vector",target_values_vector_size, predicted_values_vector_size);
		return 0.0;
	}
	A=target_values_vector;
	P=predicted_values_vector;

	R=mlfw_subtract_double_column_vector(A,P,NULL);
	if(mlfw_error())
	{
		return 0.0;	
	}
	RT=mlfw_column_vec_double_transpose(R,NULL);	
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		return 0.0;	
	}

	RTR=mlfw_multiply_double_row_vector_with_column_vector(RT,R,NULL);
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		return 0.0;	
	}
	SSR=mlfw_column_vec_double_get(RTR,0);
	if(mlfw_error()) return 0.0;
	mean_of_actuals=mlfw_column_vec_double_get_mean(A);
	M=mlfw_column_vec_double_create_new_filled(target_values_vector_size,mean_of_actuals,NULL);
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		mlfw_column_vec_double_destroy(RTR);
		return 0.0;	
	}
	AM=mlfw_subtract_double_column_vector(A,M,NULL);
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		mlfw_column_vec_double_destroy(RTR);
		mlfw_column_vec_double_destroy(M);
		return 0.0;
	}

	AMT=mlfw_column_vec_double_transpose(AM,NULL);
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		mlfw_column_vec_double_destroy(RTR);
		mlfw_column_vec_double_destroy(M);
		mlfw_column_vec_double_destroy(AM);
		return 0.0;
	}
	AMTAM=mlfw_multiply_double_row_vector_with_column_vector(AMT,AM,NULL);
	if(mlfw_error())
	{
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		mlfw_column_vec_double_destroy(RTR);
		mlfw_column_vec_double_destroy(M);
		mlfw_column_vec_double_destroy(AM);
		mlfw_row_vec_double_destroy(AMT);
		return 0.0;

	}
	SST=mlfw_column_vec_double_get(AMTAM,0);
	r2score=1-(SSR/SST);
		mlfw_column_vec_double_destroy(R);
		mlfw_row_vec_double_destroy(RT);
		mlfw_column_vec_double_destroy(RTR);
		mlfw_column_vec_double_destroy(M);
		mlfw_column_vec_double_destroy(AM);
		mlfw_row_vec_double_destroy(AMT);
		mlfw_column_vec_double_destroy(AMTAM);
return r2score;
}
