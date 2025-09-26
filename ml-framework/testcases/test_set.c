#include<stdio.h>
#include<dmlfw_error.h>
#include<dmlfw_set.h>
int main()
{
	dmlfw_set_string *s;
	char err[512];
	char dbg[512];
	s=dmlfw_set_string_create_new();
	if(dmlfw_error())
	{
		printf("Unable to create set\n");
		printf("Error code : %u\n",dmlfw_get_error_code());
		dmlfw_get_error_string(err,512);
		dmlfw_get_debug_string(dbg,512);
		printf("Error : %s\n",err);
		printf("Debug : %s\n",dbg);
		return 0;
	}
	dmlfw_set_string_destroy(NULL);
	if(dmlfw_error())
	{
		printf("Unable to create set\n");
		printf("Error code : %u\n",dmlfw_get_error_code());
		dmlfw_get_error_string(err,512);
		dmlfw_get_debug_string(dbg,512);
		printf("Error : %s\n",err);
		printf("Debug : %s\n",dbg);
		return 0;
	}
	dmlfw_set_string_destroy(s);
	return 0;
}
