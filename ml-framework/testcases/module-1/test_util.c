#include<stdio.h>
#include<dmlfw_utils.h>
int main(int argc,char *argv[])
{
	printf("%d\n",dmlfw_strcmp_case_insensitive(argv[1],argv[2]));
	return 0;
}
