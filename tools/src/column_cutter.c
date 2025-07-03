#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_utils.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	mlfw_mat_string *matrix;
	mlfw_mat_string *m;
	mlfw_row_vec_string *matrix_header;
	mlfw_row_vec_string *m_header;
	dimension_t indexes_size;
	index_t *indexes;
	dimension_t matrix_header_size;
	dimension_t m_header_size;
	index_t i,j,k;

	if(argc<=3)
	{
		printf("Usage [column_cutter source_csv target_csv column_to_cut_1 column_to_cut_2 ....]\n");
		return 0;
	}
	matrix=mlfw_mat_string_from_csv(argc[1],NULL,&matrix_header);
	if(matrix==NULL)
	{
		printf("Unable to load %s\n",argv[1]);
		return 0;
	}
	indexes_size=argc-3;
	indexes=(index_t *)malloc(sizeof(index_t)*indexes_size);
	if(indexes==NULL)
	{
		printf("Low memory\n");
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	matrix_header_size=mlfw_row_vec_string_get_size(matrix_header);
	i=3;
	k=0;
	while(i<argc)
	{
		for(j=0;j<matrix_header_size;++j)
		{
			mlfw_row_vec_string_get(matrix_header,j,&str);
			if(str==NULL)
			{
				printf("Low memory\n");
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(matrix_header);
				free(indexes);
				return 0;
			}
			if(mlfw_strcmp_case_insensitive(argv[i],str)==0)
			{
				free(str);
				indexes[k]=j;
				k++;
				break;
			}
			free(str);
		}
		if(j==matrix_header_size) //argv[i] not in matrix_header
		{
			printf("%s column does not exists\n",argv[i]);
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(matrix_header);
			free(indexes);
			return 0;
		}
		i++;
	}

	m=mlfw_mat_string_delete_columns(matrix,indexes,indexes_size,NULL);
	if(m==NULL)
	{
		printf("Low memory\n");
		free(indexes);
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	mlfw_mat_string_destroy(matrix);
	
	m_header_size=matrix_header_size-indexes_size;
	m_header=mlfw_row_vec_string_create_new(m_header_size);
	if(m_header==NULL)
	{	
		printf("Low memory\n");
		free(indexes);
		mlfw_row_vec_string_destroy(matrix_header);
		mlfw_mat_string_destroy(m);
		return 0;
	}	
	
	// copy the column names from the matrix_header to m_header, ignore the cut ones
	k=0;
	for(i=0;i<matrix_header;++i)
	{
		for(j=0;j<indexes_size;++j)
		{
			if(indexes[j]==i) break; //found the column index in indexes, which needs to be ignored
		}
		if(j<indexes_size) continue; // ignore this column
		mlfw_row_vec_string_get(matrix_header,i,&str);
		if(str==NULL)
		{
			printf("Low memory\n");
			free(indexes);
			mlfw_mat_string_destroy(m);
			mlfw_row_vec_string_destroy(m_header);
			mlfw_rowvec_string_destory(matrix_header);
			return 0;
		}
		mlfw_mat_string_set(m_header,k,str);
		k++;
		free(str);
	}
	mlfw_mat_string_to_csv(m,argv[2],m_header);
	free(indexes);
	mlfw_mat_string_destroy(m);
	mlfw_row_vec_string_destroy(m_header);
	mlfw_row_vec_string_destroy(matrix_header);
	return 0;
}
