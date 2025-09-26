#include<stdio.h>
#include<dmlfw_set.h>
#include<stdlib.h>
int main()
{
	dmlfw_set_string *set;
	index_t i;
	char *str;
	dimension_t size;
	set=dmlfw_set_string_create_new();
	dmlfw_set_string_add(set,"Good");
	dmlfw_set_string_add(set,"Bad");
	dmlfw_set_string_add(set,"good");
	dmlfw_set_string_add(set,"Nice");
	dmlfw_set_string_add(set,"Good");
	dmlfw_set_string_add(set,"Bad");
	dmlfw_set_string_add(set,"Nice");

	size=dmlfw_set_string_get_size(set);
	printf("Size of set is %u\n",size);
	for(i=0;i<size;++i)
	{
		dmlfw_set_string_get(set,i,&str);
		if(str!=NULL)
		{
			printf("%s\n",str);
			free(str);
		}
	}
	dmlfw_set_string_destroy(set);
	return 0;
}
