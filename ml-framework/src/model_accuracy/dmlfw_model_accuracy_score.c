#include<dmlfw_model_accuracy.h>
#include<dmlfw_vector.h>
#include<dmlfw_matrix.h>
#include<dmlfw_operations.h>
#include<stdio.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

double dmlfw_get_r2_score(dmlfw_column_vec_double *target_values_vector,dmlfw_column_vec_double *predicted_values_vector)
{
	dimension_t target_values_vector_size;
	dimension_t predicted_values_vector_size;
	dmlfw_column_vec_double *A;
	dmlfw_column_vec_double *P;
	dmlfw_column_vec_double *R; //  A-P
	dmlfw_row_vec_double *RT; // transpose of R
	dmlfw_column_vec_double *RTR; // RT*R
	double mean_of_actuals;
	dmlfw_column_vec_double *M;
	dmlfw_column_vec_double *AM; // A-M
	dmlfw_row_vec_double *AMT; // transpose of AM
	dmlfw_column_vec_double *AMTAM; // AMT*AM
	
	double SSR;
	double SST;
	double r2score;

	dmlfw_reset_error();

    if (target_values_vector == NULL) {
        _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "target_values_vector");
        return 0.0;
    }
    if (predicted_values_vector == NULL) {
        _dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "predicted_values_vector");
        return 0.0;
    }
	target_values_vector_size=dmlfw_column_vec_double_get_size(target_values_vector);
	predicted_values_vector_size=dmlfw_column_vec_double_get_size(predicted_values_vector);
	if(target_values_vector_size!=predicted_values_vector_size)
	{
		 _dmlfw_set_error(MLFW_VECTOR_SIZE_NOT_SAME_CODE, MLFW_VECTOR_SIZE_NOT_SAME,"target_values_vector", "predicted_values_vector",target_values_vector_size, predicted_values_vector_size);
		return 0.0;
	}
	A=target_values_vector;
	P=predicted_values_vector;

	R=dmlfw_subtract_double_column_vector(A,P,NULL);
	if(dmlfw_error())
	{
		return 0.0;	
	}
	RT=dmlfw_column_vec_double_transpose(R,NULL);	
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		return 0.0;	
	}

	RTR=dmlfw_multiply_double_row_vector_with_column_vector(RT,R,NULL);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		return 0.0;	
	}
	SSR=dmlfw_column_vec_double_get(RTR,0);
	if(dmlfw_error()) return 0.0;
	mean_of_actuals=dmlfw_column_vec_double_get_mean(A);
	M=dmlfw_column_vec_double_create_new_filled(target_values_vector_size,mean_of_actuals,NULL);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		return 0.0;	
	}
	AM=dmlfw_subtract_double_column_vector(A,M,NULL);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		return 0.0;
	}

	AMT=dmlfw_column_vec_double_transpose(AM,NULL);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		return 0.0;
	}
	AMTAM=dmlfw_multiply_double_row_vector_with_column_vector(AMT,AM,NULL);
	if(dmlfw_error())
	{
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		dmlfw_row_vec_double_destroy(AMT);
		return 0.0;

	}
	SST=dmlfw_column_vec_double_get(AMTAM,0);
	r2score=1-(SSR/SST);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		dmlfw_row_vec_double_destroy(AMT);
		dmlfw_column_vec_double_destroy(AMTAM);
return r2score;
}
