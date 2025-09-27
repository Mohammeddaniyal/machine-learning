/**
 * @file column_cutter.c
 * @brief Command-line tool to cut specified columns from a CSV file.
 * @ingroup ml-tools-dataset
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 *
 * This tool loads a CSV file into a string matrix, removes specified columns by name,
 * and saves the resulting matrix to a new CSV file. It helps extract subsets of data for
 * preprocessing or analysis using the ml-framework matrix and utility modules.
 *
 * Usage:
 *   ./column_cutter source.csv target.csv column_to_cut_1 column_to_cut_2 ...
 * 
 * Parameters:
 *   - source_csv: Path to the input CSV file.
 *   - target_csv: Path to the output CSV file with specified columns removed.
 *   - columns_to_cut: List of column names to remove from the CSV.
 *
 * This tool uses ml-framework headers: dmlfw_matrix.h, dmlfw_vector.h, dmlfw_utils.h.
 */
#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_utils.h>
#include<stdio.h>
#include<stdlib.h>
/**
 * @brief Main entry point for the column cutter tool.
 *
 * Loads the CSV into an ml-framework string matrix, identifies column indices to remove,
 * deletes those columns, and writes the reduced matrix back to CSV.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument vector.
 * @return Returns 0 on success or with usage/error messages.
 *
 * Example:
 * @code
 * ./column_cutter input.csv output.csv Gender Age
 * @endcode
 */
int main(int argc,char *argv[])
{
	char *str;
	dmlfw_mat_string *matrix;
	dmlfw_mat_string *m;
	dmlfw_row_vec_string *matrix_header;
	dmlfw_row_vec_string *m_header;
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
	matrix=dmlfw_mat_string_from_csv(argv[1],NULL,&matrix_header);
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
		dmlfw_mat_string_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	matrix_header_size=dmlfw_row_vec_string_get_size(matrix_header);
	i=3;
	k=0;
	while(i<argc)
	{
		for(j=0;j<matrix_header_size;++j)
		{
			dmlfw_row_vec_string_get(matrix_header,j,&str);
			if(str==NULL)
			{
				printf("Low memory\n");
				dmlfw_mat_string_destroy(matrix);
				dmlfw_row_vec_string_destroy(matrix_header);
				free(indexes);
				return 0;
			}
			if(dmlfw_strcmp_case_insensitive(argv[i],str)==0)
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
			dmlfw_mat_string_destroy(matrix);
			dmlfw_row_vec_string_destroy(matrix_header);
			free(indexes);
			return 0;
		}
		i++;
	}

	m=dmlfw_mat_string_delete_columns(matrix,indexes,indexes_size,NULL);
	if(m==NULL)
	{
		printf("Low memory\n");
		free(indexes);
		dmlfw_mat_string_destroy(matrix);
		dmlfw_row_vec_string_destroy(matrix_header);
		return 0;
	}
	dmlfw_mat_string_destroy(matrix);
	
	m_header_size=matrix_header_size-indexes_size;
	m_header=dmlfw_row_vec_string_create_new(m_header_size);
	if(m_header==NULL)
	{	
		printf("Low memory\n");
		free(indexes);
		dmlfw_row_vec_string_destroy(matrix_header);
		dmlfw_mat_string_destroy(m);
		return 0;
	}	
	
	// copy the column names from the matrix_header to m_header, ignore the cut ones
	k=0;
	for(i=0;i<matrix_header_size;++i)
	{
		for(j=0;j<indexes_size;++j)
		{
			if(indexes[j]==i) break; //found the column index in indexes, which needs to be ignored
		}
		if(j<indexes_size) continue; // ignore this column
		dmlfw_row_vec_string_get(matrix_header,i,&str);
		if(str==NULL)
		{
			printf("Low memory\n");
			free(indexes);
			dmlfw_mat_string_destroy(m);
			dmlfw_row_vec_string_destroy(m_header);
			dmlfw_row_vec_string_destroy(matrix_header);
			return 0;
		}
		dmlfw_row_vec_string_set(m_header,k,str);
		k++;
		free(str);
	}
	dmlfw_mat_string_to_csv(m,argv[2],m_header);
	free(indexes);
	dmlfw_mat_string_destroy(m);
	dmlfw_row_vec_string_destroy(m_header);
	dmlfw_row_vec_string_destroy(matrix_header);
	return 0;
}
/** @} */
