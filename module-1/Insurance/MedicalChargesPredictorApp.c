#include<mlfw_matrix.h>
#include<mlfw_vector.h>
#include<mlfw_scale.h>
#include<mlfw_set.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void clearBuffer()
{
	while(getchar()!='\n');
}
int main()
{
	int age;
	char gender;
	double body_mass_index;
	int number_of_children;
	char smokes;
	char region[52];
	int succ;
	char found;	
	mlfw_set_string *regions_set;
	char *str;
	dimension_t set_size;
	index_t i;

	double sex_male=0;
	double sex_female=0;
	
	double smokes_yes=0;
	double smokes_no=0;

	double region_northwest=0;
	double region_northeast=0;
	double region_southwest=0;
	double region_southeast=0;

	char age_string[5];
	char bmi_string[7];
	char number_of_children_string[5];
	printf("Enter age : ");
	scanf("%d",&age);
	clearBuffer();
	if(age<0 || age>120)
	{
		printf("Invalid age! must in between 0-120\n");
		return 0;
	}
	printf("Enter gender (M/F) : ");
	scanf(" %c",&gender);
	clearBuffer();
	if(gender!='m' && gender!='M' && gender!='f' && gender!='F')
	{
		printf("Invalid gender (M/F)\n");
		return 0;
	}
	if(gender=='m' || gender=='M') sex_male=1;
	else sex_female=1;
	printf("Enter body mass index : ");
	scanf("%lf",&body_mass_index);
	clearBuffer();
	if(body_mass_index<10 || body_mass_index>60)
	{
		printf("Invalid body mass index, must in between 10 - 60\n");
		return 0;
	}
	printf("Enter number of children : ");
	scanf("%d",&number_of_children);
	clearBuffer();
	if(number_of_children<0)
	{
		printf("Invalid input \n");
		return 0;
	}

	printf("Enter Smokes (Y/N) : ");
	scanf(" %c",&smokes);
	clearBuffer();
	if(smokes!='y' && smokes!='Y' && smokes!='n' && smokes!='N')
	{
		printf("Invalid imput (Y/N)\n");
		return 0;
	}
	if(smokes=='y' || smokes=='Y') smokes_yes=1;
	else smokes_no=1;
	printf("Enter region : ");
	fgets(region,52,stdin);
	region[strlen(region)-1]='\0';
	regions_set=mlfw_set_string_create_new();
	if(regions_set==NULL)
	{
		printf("Low memory\n");
		return 0;
	}
	succ=mlfw_set_string_add(regions_set,"northeast");
	if(succ==-1)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(regions_set);
		return 0;
	}
	succ=mlfw_set_string_add(regions_set,"northwest");
	if(succ==-1)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(regions_set);
		return 0;
	}
	succ=mlfw_set_string_add(regions_set,"southeast");
	if(succ==-1)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(regions_set);
		return 0;
	}
	succ=mlfw_set_string_add(regions_set,"southwest");
	if(succ==-1)
	{
		printf("Low memory\n");
		mlfw_set_string_destroy(regions_set);
		return 0;
	}

	set_size=mlfw_set_string_get_size(regions_set);
	found=0;	
	// lower case the input region
	for(i=0;region[i];++i)
	{
		if(region[i]>=65 && region[i]<=90) region[i]+=32;	
	}
	printf("Region %s\n",region);
	for(i=0;i<set_size;++i)
	{
		mlfw_set_string_get(regions_set,i,&str);
		if(str==NULL)
		{
			printf("Low memory\n");
			mlfw_set_string_destroy(regions_set);
			return 0;
		}
		if(strcmp(region,str)==0) 
		{	
			found=1;
			free(str);
			break;
		}
		free(str);
	}
		
	if(found==0)
	{
		printf("Invalid region must be (northeast/northwest/southeast/southwest) \n");
		mlfw_set_string_destroy(regions_set);
		return 0;
	}
	
	printf("%d %lf %c %d %c %s\n",age,body_mass_index,gender,number_of_children,smokes,region);

	snprintf(age_string,sizeof(age_string),"%d",age);
	snprintf(bmi_string,sizeof(bmi_string),"%2.2lf",body_mass_index);
	snprintf(number_of_children_string,sizeof(number_of_children_string),"%d",number_of_children);

	printf("%s %s %s\n",age_string,bmi_string,number_of_children_string);

	
	
	
	mlfw_set_string_destroy(regions_set);
	return 0;
}
