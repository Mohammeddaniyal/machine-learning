#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_operations.h>
#include<stdio.h>

void print_matrix(mlfw_mat_double *matrix)
{
	dimension_t rows,columns;
	index_t r,c;
	mlfw_mat_double_get_dimensions(matrix,&rows,&columns);
	for(r=0;r<rows;++r)
	{
		for(c=0;c<columns;++c)
		{
			printf("%lf ",mlfw_mat_double_get(matrix,r,c));
		}
		printf("\n");
	}
}
int main(int argc,char *argv[])
{
	mlfw_mat_double *m1,*m2,*m3;
	mlfw_row_vec_string *h1,*h2;

	if(argc!=3)
	{
		printf("Pass name of 2 csv files with matrix as command line arguments\n");
		return 0;
	}
	m1=mlfw_mat_double_from_csv(argv[1],NULL,&h1);
	if(m1==NULL)
	{
		printf("Unable to load matrix from %s\n",argv[1]);
		return 0;
	}
	m2=mlfw_mat_double_from_csv(argv[2],NULL,&h2);
	if(m2==NULL)
	{
		printf("Unable to load matrix from %s\n",argv[2]);
		mlfw_mat_double_destroy(m1);
		mlfw_row_vec_string_destroy(h1);
		return 0;
	}
	m3=mlfw_multiply_double_matrix_with_matrix_strassens(m1,m2,NULL);
	if(m3==NULL)
	{
		printf("low memory\n");
		mlfw_mat_double_destroy(m1);
		mlfw_mat_double_destroy(m2);
		mlfw_row_vec_string_destroy(h1);
		mlfw_row_vec_string_destroy(h2);
		return 0;
	}
	print_matrix(m3);
	mlfw_mat_double_destroy(m1);
	mlfw_mat_double_destroy(m2);
	mlfw_mat_double_destroy(m3);
	mlfw_row_vec_string_destroy(h1);
	mlfw_row_vec_string_destroy(h2);
	return 0;
}
