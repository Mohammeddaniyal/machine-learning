#include<stdio.h>
#include<mlfw_set.h>
#include<stdlib.h>
int main()
{
	mlfw_set_string *set;
	index_t i;
	char *str;
	dimension_t size;
	set=mlfw_set_string_create_new();
	mlfw_set_string_add(set,"Good");
	mlfw_set_string_add(set,"Bad");
	mlfw_set_string_add(set,"good");
	mlfw_set_string_add(set,"Nice");
	mlfw_set_string_add(set,"Good");
	mlfw_set_string_add(set,"Bad");
	mlfw_set_string_add(set,"Nice");

	size=mlfw_set_string_get_size(set);
	printf("Size of set is %u\n",size);
	for(i=0;i<size;++i)
	{
		mlfw_set_string_get(set,i,&str);
		if(str!=NULL)
		{
			printf("%s\n",str);
			free(str);
		}
	}
	mlfw_set_string_destroy(set);
	return 0;
}
