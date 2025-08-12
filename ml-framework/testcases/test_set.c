#include<stdio.h>
#include<mlfw_error.h>
#include<mlfw_set.h>
int main()
{
	mlfw_set_string *s;
	char err[512];
	char dbg[512];
	s=mlfw_set_string_create_new();
	if(mlfw_error())
	{
		printf("Unable to create set\n");
		printf("Error code : %u\n",mlfw_get_error_code());
		mlfw_get_error_string(err,512);
		mlfw_get_debug_string(dbg,512);
		printf("Error : %s\n",err);
		printf("Debug : %s\n",dbg);
		return 0;
	}
	mlfw_set_string_destroy(s);
	{
		printf("Unable to create set\n");
		printf("Error code : %u\n",mlfw_get_error_code());
		mlfw_get_error_string(err,512);
		mlfw_get_debug_string(dbg,512);
		printf("Error : %s\n",err);
		printf("Debug : %s\n",dbg);
		return 0;
	}
	return 0;
}
