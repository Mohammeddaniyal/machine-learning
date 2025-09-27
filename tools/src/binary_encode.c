/**
 * @file binary_encode.c
 * @brief Command-line tool to encode specified CSV columns into binary format.
 * @ingroup ml-tools-encoding
 * @{
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 * @version 1.0
 *
 * This tool reads an input CSV file and converts specified columns to binary encoding,
 * which is useful for preprocessing categorical features for machine learning models
 * using the ml-framework's encoder module.
 *
 * Usage:
 *   ./binary_encode source.csv target.csv column1 column2 ...
 * 
 * Parameters:
 *   - source_csv: Path to the input CSV file.
 *   - target_csv: Path to the output CSV file where encoded data will be saved.
 *   - columns_to_encode: List of column names to apply binary encoding on.
 *
 * This tool uses the functions from the ml-framework encoder and vector modules
 * (`dmlfw_encoder.h`, `dmlfw_vector.h`).
 *
 * @author Mohammed Daniyal
 * @date 2025-09-26
 */

#include<stdio.h>
#include<dmlfw_encoder.h>
#include<dmlfw_vector.h>

/**
 * @brief Main entry point for the binary encoding tool.
 *
 * Parses command-line arguments for input/output CSV files and column names,
 * creates a vector of columns to encode, and applies binary encoding on the specified columns.
 *
 * @param argc Argument count.
 * @param argv Argument vector.
 * @return Returns 0 on successful encoding, or 0 with usage info or memory error message.
 *
 * Example:
 * @code
 * ./binary_encode data.csv encoded_data.csv Gender Occupation
 * @endcode
 */
int main(int argc,char *argv[])
{
    dmlfw_row_vec_string *columns_to_encode;
    dimension_t number_of_columns_to_encode;
    char *source_csv;
    char *target_csv;
    index_t i;

    if(argc <= 3)
    {
        printf("Usage [binary_encoder source_csv target_csv column_1_to_encode columns_2_to_encode ....]\n");
        return 0;
    }

    source_csv = argv[1];
    target_csv = argv[2];
    number_of_columns_to_encode = argc - 3;
    columns_to_encode = dmlfw_row_vec_string_create_new(number_of_columns_to_encode);

    if(columns_to_encode == NULL)
    {
        printf("Low memory\n");
        return 0;
    }

    for(i = 3; i < argc; ++i)
    {
        dmlfw_row_vec_string_set(columns_to_encode, i - 3, argv[i]);
    }

    dmlfw_encoder_encode_binary(source_csv, target_csv, columns_to_encode);
    return 0;
}
/** @} */
