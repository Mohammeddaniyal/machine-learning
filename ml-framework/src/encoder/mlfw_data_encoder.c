#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<mlfw_vector.h>
#include<mlfw_matrix.h>
#include<mlfw_encoder.h>
#include<mlfw_set.h>
#include<mlfw_utils.h>
void mlfw_encoder_encode_one_hot(char *source,char *target,mlfw_row_vec_string *columns_to_encode)
{
	int *encode_columns;
	index_t size;
	char *ptr1,*ptr2;
	uint8_t found;
	FILE *target_file;
	mlfw_mat_string *matrix;
	mlfw_set_string **sets;
	char *string;
	char *set_string;
	dimension_t set_size;
	index_t r,c;
	index_t data_start_row_index;
	index_t matrix_rows,matrix_columns;
	index_t i,j;
	mlfw_row_vec_string *header;
	dimension_t header_size;
	if(source==NULL || target==NULL || columns_to_encode==NULL) return;
	// passing null so that the function create one for us and give a filled one
	matrix=mlfw_mat_string_from_csv(source,NULL,&header);
	if(matrix==NULL) return;
	// create array of indexes against encode_columns
	
	size=mlfw_row_vec_string_get_size(columns_to_encode);
	encode_columns=(int *)malloc(sizeof(int)*size);
	if(encode_columns==NULL)
	{
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		return;
	}
	// logic to create array of indexes starts here
	// iterate the columns to encode
	// one by one search for column name from columns_to_encode in header
	// and grap the index
	header_size=mlfw_row_vec_string_get_size(header);

	for(i=0;i<size;++i)
	{
		mlfw_row_vec_string_get(columns_to_encode,i,&ptr1);
		if(ptr1==NULL)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
		found=0;
		for(j=0;j<header_size;j++)
		{
			mlfw_row_vec_string_get(header,j,&ptr2);
			if(ptr2==NULL)
			{
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				free(ptr1);
				return;
			}
			if(mlfw_strcmp_case_insensitive(ptr1,ptr2)==0)
			{
				found=1;
			}	
			free(ptr2);
			if(found==1) break;
		}
		free(ptr1);
		if(found==0)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
		encode_columns[i]=j;
	}
	// logic to create array of indexes ends here
	mlfw_mat_string_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	// verifying the correct column indexes
	for(i=0;i<size;++i)
	{
		c=encode_columns[i];
		if(c<0 || c>=matrix_columns)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
	}
	sets=(mlfw_set_string **)malloc(sizeof(mlfw_set_string *)*size);
	if(sets==NULL)
	{
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		free(encode_columns);
		return;
	}
	for(i=0;i<size;++i)
	{
		sets[i]=mlfw_set_string_create_new();
		if(sets[i]==NULL)
		{
			for(j=0;j<i;++j)
			{
				mlfw_set_string_destroy(sets[j]);
			}
			free(sets);
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
	}
	data_start_row_index=0;


	// Populating the sets with columns_to_encode values,
	// like male,female,yes,no, so that it can be added as suffix in updated
	// column name as like sex_male,smoker_no
	// each sets[i] represents a column values
	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(i=0;i<size;++i) // loop on the encoded columns
		{
			c=encode_columns[i];
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(j=0;j<i;++j)
				{
					mlfw_set_string_destroy(sets[j]);
				}
				free(sets);
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				return;
			}
			else
			{
				if(mlfw_set_string_add(sets[i],string)==-1)
				{
					free(string);
					for(j=0;j<i;++j)
					{
						mlfw_set_string_destroy(sets[j]);
					}
					free(sets);
					mlfw_mat_string_destroy(matrix);
					mlfw_row_vec_string_destroy(header);
					free(encode_columns);
					return;
				}
				free(string);
			}

		} // inner-for loop ends
	} // outer-for loop ends

	// code to test the contents of the set
	/*for(i=0;i<size;++i)
	{
		set_size=mlfw_set_string_get_size(sets[i]);
		for(j=0;j<set_size;++j)
		{
			mlfw_set_string_get(sets[i],j,&string);
			if(string!=NULL)
			{
				printf("%s\n",string);
				free(string);
			}
		}
	}*/
	
       // logic to create the file starts here	
	target_file=fopen(target,"w");
	if(target_file==NULL)
	{
		for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
		free(sets);
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		free(encode_columns);
		return;
	}
	// encoding the header starts here
	for(c=0;c<header_size;++c)
		{
			mlfw_row_vec_string_get(header,c,&string);
			if(string==NULL)
			{
			for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
			free(sets);
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			fclose(target_file);
			target_file=fopen(target,"w"); // create blank file, reason to erase all the written 
			fclose(target_file);
			return;
			}
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size) // found, cth column is to be hot encoded
			{
				set_size=mlfw_set_string_get_size(sets[i]);
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
					if(set_string==NULL)
					{
					free(string);
					for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
					free(sets);
					mlfw_mat_string_destroy(matrix);
					mlfw_row_vec_string_destroy(header);
					free(encode_columns);
					fclose(target_file);
					target_file=fopen(target,"w"); 
					fclose(target_file);
					return;
					}
					fputs(string,target_file);
					fputc('_',target_file);
					fputs(set_string,target_file);
					free(set_string);
					if(j<set_size-1) fputc(',',target_file);
				}
			}
			else // not found, the column not to be hot encoded
			{
			fputs(string,target_file);
			}
			free(string);
			if(c==matrix_columns-1) fputc('\n',target_file);
			else fputc(',',target_file);
		}
	// encoding the header ends here

	// encode data
	
	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(c=0;c<matrix_columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
				free(sets);
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				fclose(target_file);
				target_file=fopen(target,"w"); // create blank file, reason to erase all the written 
				fclose(target_file);
				return;
			}
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size) // found, cth column is to be hot encoded
			{
				set_size=mlfw_set_string_get_size(sets[i]);
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
					if(set_string==NULL)
					{
					free(string);
					for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
					free(sets);
					mlfw_mat_string_destroy(matrix);
					mlfw_row_vec_string_destroy(header);
					free(encode_columns);
					fclose(target_file);
					target_file=fopen(target,"w"); 
					fclose(target_file);
					return;
					}
					if(strcmp(string,set_string)==0)
					{
						fputc('1',target_file);
					}
					else
					{
						fputc('0',target_file);
					}
					free(set_string);
					if(j<set_size-1) fputc(',',target_file);
				}
			}
			else // not found, the column not to be hot encoded
			{
			fputs(string,target_file);
			}
			free(string);
			if(c==matrix_columns-1) fputc('\n',target_file);
			else fputc(',',target_file);
		}
	
	}
	for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
	free(sets);	
	mlfw_mat_string_destroy(matrix);	
	mlfw_row_vec_string_destroy(header);
	free(encode_columns);
	fclose(target_file);
}

void mlfw_encoder_encode_binary(char *source,char *target,mlfw_row_vec_string *columns_to_encode)
{
	char binary_string[33]; // 1 extra for \0
	int bi;
	uint32_t largest_code;
	int bits_required;
	char set_file_name[1025];
	FILE *set_file;
	
	int *encode_columns;
	index_t size;
	char *ptr1,*ptr2;
	uint8_t found;
	FILE *target_file;
	mlfw_mat_string *matrix;
	mlfw_set_string **sets;
	char *string;
	char *set_string;
	dimension_t set_size;
	index_t r,c;
	index_t data_start_row_index;
	index_t matrix_rows,matrix_columns;
	index_t i,j;
	mlfw_row_vec_string *header;
	dimension_t header_size;
	if(source==NULL || target==NULL || columns_to_encode==NULL) return;
	// passing null so that the function create one for us and give a filled one
	matrix=mlfw_mat_string_from_csv(source,NULL,&header);
	if(matrix==NULL) return;
	// create array of indexes against encode_columns
	
	size=mlfw_row_vec_string_get_size(columns_to_encode);
	encode_columns=(int *)malloc(sizeof(int)*size);
	if(encode_columns==NULL)
	{
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		return;
	}
	// logic to create array of indexes starts here
	// iterate the columns to encode
	// one by one search for column name from columns_to_encode in header
	// and grap the index
	header_size=mlfw_row_vec_string_get_size(header);

	for(i=0;i<size;++i)
	{
		mlfw_row_vec_string_get(columns_to_encode,i,&ptr1);
		if(ptr1==NULL)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
		found=0;
		for(j=0;j<header_size;j++)
		{
			mlfw_row_vec_string_get(header,j,&ptr2);
			if(ptr2==NULL)
			{
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				free(ptr1);
				return;
			}
			if(mlfw_strcmp_case_insensitive(ptr1,ptr2)==0)
			{
				found=1;
			}	
			free(ptr2);
			if(found==1) break;
		}
		free(ptr1);
		if(found==0)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
		encode_columns[i]=j;
	}
	// logic to create array of indexes ends here
	mlfw_mat_string_get_dimensions(matrix,&matrix_rows,&matrix_columns);
	// verifying the correct column indexes
	for(i=0;i<size;++i)
	{
		c=encode_columns[i];
		if(c<0 || c>=matrix_columns)
		{
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
	}
	sets=(mlfw_set_string **)malloc(sizeof(mlfw_set_string *)*size);
	if(sets==NULL)
	{
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		free(encode_columns);
		return;
	}
	for(i=0;i<size;++i)
	{
		sets[i]=mlfw_set_string_create_new();
		if(sets[i]==NULL)
		{
			for(j=0;j<i;++j)
			{
				mlfw_set_string_destroy(sets[j]);
			}
			free(sets);
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			return;
		}
	}
	data_start_row_index=0;


	// Populating the sets with columns_to_encode values,
	// like male,female,yes,no, so that it can be added as suffix in updated
	// column name as like sex_male,smoker_no
	// each sets[i] represents a column values
	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(i=0;i<size;++i) // loop on the encoded columns
		{
			c=encode_columns[i];
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(j=0;j<i;++j)
				{
					mlfw_set_string_destroy(sets[j]);
				}
				free(sets);
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				return;
			}
			else
			{
				if(mlfw_set_string_add(sets[i],string)==-1)
				{
					free(string);
					for(j=0;j<i;++j)
					{
						mlfw_set_string_destroy(sets[j]);
					}
					free(sets);
					mlfw_mat_string_destroy(matrix);
					mlfw_row_vec_string_destroy(header);
					free(encode_columns);
					return;
				}
				free(string);
			}

		} // inner-for loop ends
	} // outer-for loop ends

      // logic to create the file starts here	
	target_file=fopen(target,"w");
	if(target_file==NULL)
	{
		for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
		free(sets);
		mlfw_mat_string_destroy(matrix);
		mlfw_row_vec_string_destroy(header);
		free(encode_columns);
		return;
	}
	// encoding the header starts here
	for(c=0;c<header_size;++c)
		{
			mlfw_row_vec_string_get(header,c,&string);
			if(string==NULL)
			{
			for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
			free(sets);
			mlfw_mat_string_destroy(matrix);
			mlfw_row_vec_string_destroy(header);
			free(encode_columns);
			fclose(target_file);
			target_file=fopen(target,"w"); // create blank file, reason to erase all the written 
			fclose(target_file);
			return;
			}
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size) // found, cth column is to be hot encoded
			{
				set_size=mlfw_set_string_get_size(sets[i]);
				
				largest_code=set_size-1;
				mlfw_uint32_to_binary(largest_code,binary_string);
				for(bi=0;binary_string[bi]=='0';++bi);
				bits_required=32-bi;

				for(bi=1;bi<=bits_required;++bi)
				{
					fputs(string,target_file);
					fputc('_',target_file);
					fputc((char)(bi+48),target_file);
					if(bi<bits_required) fputc(',',target_file);
				}
			
			}
			else // not found, the column not to be hot encoded
			{
			fputs(string,target_file);
			}
			free(string);
			if(c==matrix_columns-1) fputc('\n',target_file);
			else fputc(',',target_file);
		}
	// encoding the header ends here

	// encode data
	
	for(r=data_start_row_index;r<matrix_rows;++r)
	{
		for(c=0;c<matrix_columns;++c)
		{
			mlfw_mat_string_get(matrix,r,c,&string);
			if(string==NULL)
			{
				for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
				free(sets);
				mlfw_mat_string_destroy(matrix);
				mlfw_row_vec_string_destroy(header);
				free(encode_columns);
				fclose(target_file);
				target_file=fopen(target,"w"); // create blank file, reason to erase all the written 
				fclose(target_file);
				return;
			}
			for(i=0;i<size;++i)
			{
				if(c==encode_columns[i]) break;
			}
			if(i<size) // found, cth column is to be hot encoded
			{
				set_size=mlfw_set_string_get_size(sets[i]);
				largest_code=set_size-1;
				mlfw_uint32_to_binary(largest_code,binary_string);
				for(bi=0;binary_string[bi]=='0';bi++);
				bits_required=32-bi;
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
					if(set_string==NULL)
					{
					free(string);
					for(j=0;j<i;++j) mlfw_set_string_destroy(sets[j]);
					free(sets);
					mlfw_mat_string_destroy(matrix);
					mlfw_row_vec_string_destroy(header);
					free(encode_columns);
					fclose(target_file);
					target_file=fopen(target,"w"); 
					fclose(target_file);
					return;
					}
					if(strcmp(string,set_string)==0)
					{
						mlfw_uint32_to_binary(j,binary_string);
						for(bi=32-bits_required;bi<=31;bi++)
						{
							fputc(binary_string[bi],target_file);
							if(bi<31) fputc(',',target_file);
						}
					}
					free(set_string);
				}
			}
			else // not found, the column not to be hot encoded
			{
			fputs(string,target_file);
			}
			free(string);
			if(c==matrix_columns-1) fputc('\n',target_file);
			else fputc(',',target_file);
		}
	
	}
	
	for(c=0;c<header_size;++c)
	{
		for(i=0;i<size;++i)
		{
			if(c==encode_columns[i]) break;
		}
		if(i==size) continue; // not found
		// found so create a file named with header[c] and fill with sets[i] strings
		mlfw_row_vec_string_get(header,c,&string);
		if(string!=NULL)
		{
			sprintf(set_file_name,"%s.csv",string);
			set_file=fopen(set_file_name,"w");
			if(set_file==NULL) free(string);
			if(set_file!=NULL)
			{
				fprintf(set_file,"%s,code\n",string);
				free(string);
				set_size=mlfw_set_string_get_size(sets[i]);
				for(j=0;j<set_size;++j)
				{
					mlfw_set_string_get(sets[i],j,&set_string);
					if(set_string!=NULL)
					{
						fprintf(set_file,"%s,%d\n",set_string,j);
						free(set_string);
					}
				}
				fclose(set_file);
			}
		}
	}

	for(i=0;i<size;++i) mlfw_set_string_destroy(sets[i]);
	free(sets);	
	mlfw_mat_string_destroy(matrix);	
	mlfw_row_vec_string_destroy(header);
	free(encode_columns);
	fclose(target_file);
}


void mlfw_encoder_encode(char *source,char *target,mlfw_row_vec_string *columns_to_encode,char *algorithm)
{
	uint8_t algorithm_code;
	uint8_t ONE_HOT_ENCODING=1;
	uint8_t BINARY_ENCODING=2;

	if(mlfw_strcmp_case_insensitive(algorithm,"one-hot")==0)
	{
		algorithm=ONE_HOT_ENCODING;
	}else 
	if(mlfw_strcmp_case_insensitive(algorithm,"binary")==0)
	{
		algorithm=BINARY_ENCODING;
	}
	else
	{
		return; // later on we will be introducing something to notify error
	}

	if(algorithm==ONE_HOT_ENCODING)
	{
		mlfw_encoder_encode_one_hot(source,target,columns_to_encode);
	}else if(algorithm==BINARY_ENCODING)
	{
		mlfw_encoder_encode_binary(source,target,columns_to_encode);
	}
}
