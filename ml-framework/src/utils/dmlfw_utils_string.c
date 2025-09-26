#include<dmlfw_utils.h>
#include<stdio.h>
#include<dmlfw_error.h>
#include<___dmlfw_error.h>

extern __thread uint32_t _dmlfw_error_code;
extern __thread char _dmlfw_error_string[512];
extern __thread char _dmlfw_debug_string[512];

int dmlfw_strcmp_case_insensitive(const char *left,const char *right)
{
	// this function should return 0 if both are same
	// if left operand is greater then the right operand then return +ve
	// else return -ve
	char a,b;
	dmlfw_reset_error();
	if(left==NULL && right==NULL) return 0;
	if(left==NULL) 
	{  
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "left");
    	return -1;
	}
	if(right==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "right");
    	return 1;
	}
	while(*left!='\0' && *right!='\0')
	{
		a=*left;
		b=*right;
		if(a>=97 && a<=122) a-=32; // convert the case to uppercase,if lowercase
		if(b>=97 && b<=122) b-=32;
		if(a!=b) return a-b;
		left++;
		right++;
	}
	return (*left)-(*right); // to understand why?, lec Module 2 L9 7:50
}

void dmlfw_uint32_to_binary(uint32_t number,char *string)
{
	int i;
	int j;
	int bit;
	dmlfw_reset_error();
	if(string==NULL)
	{
		_dmlfw_set_error(MLFW_NULL_ARGUMENT_CODE, MLFW_NULL_ARGUMENT, "string");
		return;
	}
	for(j=0,i=31;i>=0;--i,++j)
	{
		// right shifting and AND operator
		bit = (number>>i) & 1;
		if(bit==0) string[j]='0';
		else string[j]='1';
	}
	string[j]='\0';
}
