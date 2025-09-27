/**
 * @file encode_one_hot.c
 * @brief Command-line tool for one-hot encoding specified CSV columns.
 * @ingroup ml-tools-encoding
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool reads an input CSV file and applies one-hot encoding to selected categorical columns
 * using the ml-framework's encoder capabilities, producing an output CSV with encoded features.
 *
 * Usage:
 *   ./encode_one_hot input_file output_file column_name1 column_name2 ...
 *
 * Parameters:
 *   - input_file: Path to the input CSV file.
 *   - output_file: Path to the output CSV file with one-hot encoded columns.
 *   - column_name(s): One or more column names to apply one-hot encoding.
 *
 * This tool uses the ml-framework dmlfw_encoder and dmlfw_row_vec_string modules.
 *
 */

#include<dmlfw_encoder.h>
#include<stdio.h>
#include<stdlib.h>
/**
 * @brief Main entry point for the one-hot encoding tool.
 *
 * Parses command-line arguments for input/output files and column names,
 * collects them into a vector, and calls encoder to perform one-hot encoding.
 *
 * @param argc Number of command-line arguments.
 * @param argv Command-line argument vector.
 * @return Returns 0 on success or with usage/memory error message.
 *
 * Example:
 * @code
 * ./encode_one_hot data.csv encoded_data.csv Gender Occupation
 * @endcode
 */
int main(int argc,char *argv[])
{
	char *input_file;
	char *output_file;
	
	dmlfw_row_vec_string *columns_to_encode;
	
	index_t i;
	if(argc<4)
	{
		printf("Usage : [encode_one_hot input_file output_file column_name1 column_name2]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	
	columns_to_encode=dmlfw_row_vec_string_create_new(argc-3);
	if(columns_to_encode==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	for(i=3;i<argc;i++)
	{
		dmlfw_row_vec_string_set(columns_to_encode,i-3,argv[i]);
	}
	dmlfw_encoder_encode_one_hot(input_file,output_file,columns_to_encode);
	dmlfw_row_vec_string_destroy(columns_to_encode);
	return 0;
}
/** @} */
