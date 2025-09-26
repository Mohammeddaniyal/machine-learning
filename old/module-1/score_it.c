#include<stdio.h>
#include<stdlib.h>
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_operations.h>

void score_it(char *result_file_name)
{
	dmlfw_mat_double *dataset;
	dimension_t dataset_rows,dataset_columns;
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

	dataset=dmlfw_mat_double_from_csv(result_file_name);
	if(dataset==NULL)
	{
		printf("Low memory or unable to load the results file\n");
		return;
	}
	dmlfw_mat_double_get_dimensions(dataset,&dataset_rows,&dataset_columns);

	A=dmlfw_mat_double_create_column_vec(dataset,dataset_columns-2); // 2nd last column of results file
	if(A==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		return;
	}
	P=dmlfw_mat_double_create_column_vec(dataset,dataset_columns-1); // last column of result file
	if(P==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		return;	
	}
	R=dmlfw_subtract_double_column_vector(A,P);
	if(R==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		return;	
	}
	RT=dmlfw_column_vec_double_transpose(R);	
	if(RT==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		return;	
	}

	RTR=dmlfw_multiply_double_row_vector_with_column_vector(RT,R);
	if(RTR==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		return;	
	}
	SSR=dmlfw_column_vec_double_get(RTR,0);
	mean_of_actuals=dmlfw_column_vec_double_get_mean(A);
	M=dmlfw_column_vec_double_create_new_filled(dataset_rows,mean_of_actuals);
	if(M==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		return;	
	}
	AM=dmlfw_subtract_double_column_vector(A,M);
	if(AM==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		return;
	}

	AMT=dmlfw_column_vec_double_transpose(AM);
	if(AMT==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		return;
	}
	AMTAM=dmlfw_multiply_double_row_vector_with_column_vector(AMT,AM);
	if(AMTAM==NULL)
	{
		printf("Low memory\n");
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		dmlfw_row_vec_double_destroy(AMT);
		return;

	}
	SST=dmlfw_column_vec_double_get(AMTAM,0);
	r2score=1-(SSR/SST);
	printf("R2Score of the test is %lf\n",r2score);
		dmlfw_mat_double_destroy(dataset);
		dmlfw_column_vec_double_destroy(A);
		dmlfw_column_vec_double_destroy(P);
		dmlfw_column_vec_double_destroy(R);
		dmlfw_row_vec_double_destroy(RT);
		dmlfw_column_vec_double_destroy(RTR);
		dmlfw_column_vec_double_destroy(M);
		dmlfw_column_vec_double_destroy(AM);
		dmlfw_row_vec_double_destroy(AMT);
		dmlfw_column_vec_double_destroy(AMTAM);

}

int main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("Usage : [score_it.out result_file]\n");
		return 0;
	}
	score_it(argv[1]);
	return 0;
}
