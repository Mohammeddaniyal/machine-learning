#include<mlfw_encoder.h>
#include<stdio.h>
#include<stdlib.h>
int main(int argc,char *argv[])
{
	char *input_file;
	char *output_file;
	int *encode_columns;
	dimension_t size;
	char *columns_string;
	char *s,*e;
	index_t i;
	char has_header;
	int header_exists;
	if(argc!=5)
	{
		printf("Usage : [encode_one_hot.out input_file output_file columns_to_encode has_header]\n");
		return 0;
	}
	input_file=argv[1];
	output_file=argv[2];
	columns_string=argv[3];
	has_header=argv[4][0]; // argv[4] represents a string we need its first char
	if(has_header=='Y' || has_header=='y') header_exists=1;
	else header_exists=0;	
	
	for(i=0,size=0;columns_string[i]!='\0';++i)
	{
		if(columns_string[i]==',') size++;
	}
	size++;
	encode_columns=(int *)malloc(sizeof(int)*size);
	if(encode_columns==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	i=0;
	s=columns_string;
	// this algorithm explanation 1:13:30 A20 module 1 20
	while(1)
	{
		e=s+1;
		while(*e!='\0' && *e!=',') e++;
		if(*e=='\0')
		{
			encode_columns[i]=atoi(s);
			break;
		}
		else
		{
			*e='\0';
			encode_columns[i]=atoi(s);
			++i;
			s=e+1;
		}
	}
	/*
	 * for testing the above splitting logic
	printf("Size :%u\n",size);
	for(i=0;i<size;++i)
	{
		printf("%d ",encode_columns[i]);
	}
	*/
	mlfw_encoder_encode_one_hot(input_file,output_file,encode_columns,size,header_exists);
}
