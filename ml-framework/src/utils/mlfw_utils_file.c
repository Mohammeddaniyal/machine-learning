#include<stdio.h>
#include<mlfw_error.h>
#include<___mlfw_error.h>

extern __thread uint32_t _mlfw_error_code;
extern __thread char _mlfw_error_string[512];
extern __thread char _mlfw_debug_string[512];

uint64_t mlfw_get_csv_rows_count(const char *csv_file_name)
{
	uint64_t rows;
	FILE *file;
	char m;
	mlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return 0;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
		return 0;
	}
	rows=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		else if(m=='\n') rows++;
	}
	fclose(file);
	return rows-1; // First row represents header
}
uint64_t mlfw_get_csv_columns_count(const char *csv_file_name)
{
	uint64_t columns;
	FILE *file;
	char m;
	mlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return 0;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
		return 0;
	}
	columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',') columns++;
		else if(m=='\n') break;
	}
	fclose(file);
	return columns+1; // 5 commas means 6 columns

}
void mlfw_get_csv_dimensions(const char *csv_file_name,uint64_t *rows,uint64_t *columns)
{
	FILE *file;
	char m;
	mlfw_reset_error();
	if(csv_file_name==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"csv_file_name");
		return;
	}
	if(rows==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"rows");
		return;
	}
	if(columns==NULL)
	{
		_mlfw_set_error(MLFW_NULL_ARGUMENT_CODE,MLFW_NULL_ARGUMENT,"columns");
		return;
	}
	file=fopen(csv_file_name,"r");
	if(file==NULL)
	{
		_mlfw_set_error(MLFW_UNABLE_TO_OPEN_FILE_CODE,MLFW_UNABLE_TO_OPEN_FILE,csv_file_name,"csv_file_name");
		return;
	}
	*rows=0;
	*columns=0;
	while(1)
	{
		m=fgetc(file);
		if(feof(file)) break;
		if(m=='\r') continue;
		if(m==',' && *rows==0) (*columns)++;
		else if(m=='\n') (*rows)++;
	}
	fclose(file);
	(*rows)--; // First row represents header
	(*columns)++; // 5 commas means 6 columns
}
