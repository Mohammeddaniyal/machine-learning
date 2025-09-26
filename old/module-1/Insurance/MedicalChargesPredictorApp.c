#include<dmlfw_matrix.h>
#include<dmlfw_vector.h>
#include<dmlfw_scale.h>
#include<dmlfw_operations.h>
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
	char found;	

	index_t i;

	double sex_male=0;
	double sex_female=0;
	
	double smokes_yes=0;
	double smokes_no=0;

	double region_northwest=0;
	double region_northeast=0;
	double region_southwest=0;
	double region_southeast=0;

	
	dmlfw_row_vec_double *I;
	dmlfw_mat_double *min_max_matrix;
	dmlfw_column_vec_double *m;
	dmlfw_column_vec_double *result;

	dimension_t min_max_rows,min_max_columns;
	index_t c;

	double min,max;
	double value;
	double scaled_value;

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
	found=0;	
	// lower case the input region
	for(i=0;region[i];++i)
	{
		if(region[i]>=65 && region[i]<=90) region[i]+=32;	
	}

	found=0;
	if(strcmp(region,"northeast")==0)
	{	
		found=1;
		region_northeast=1;
	}else if(strcmp(region,"southeast")==0)
	{	
		found=1;
		region_southeast=1;
	}else if(strcmp(region,"northwest")==0)
	{	
		found=1;
		region_northwest=1;
	}else if(strcmp(region,"southwest")==0)
	{	
		found=1;
		region_southwest=1;
	}  



	if(found==0)
	{
		printf("Invalid region must be (northeast/northwest/southeast/southwest) \n");
		return 0;
	}
	

	I=dmlfw_row_vec_double_create_new(12);
	if(I==NULL)
	{
		printf("Low memory\n");
		return 0;
	}

	dmlfw_row_vec_double_set(I,0,1);
	dmlfw_row_vec_double_set(I,1,(double)age);
	dmlfw_row_vec_double_set(I,2,sex_female);
	dmlfw_row_vec_double_set(I,3,sex_male);
	dmlfw_row_vec_double_set(I,4,body_mass_index);
	dmlfw_row_vec_double_set(I,5,(double)number_of_children);
	dmlfw_row_vec_double_set(I,6,smokes_yes);
	dmlfw_row_vec_double_set(I,7,smokes_no);
	dmlfw_row_vec_double_set(I,8,region_southwest);
	dmlfw_row_vec_double_set(I,9,region_southeast);
	dmlfw_row_vec_double_set(I,10,region_northwest);
	dmlfw_row_vec_double_set(I,11,region_northeast);


	min_max_matrix=dmlfw_mat_double_from_csv("min_max_scale.csv");
	if(min_max_matrix==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_double_destroy(I);
		return 0;
	}

	dmlfw_mat_double_get_dimensions(min_max_matrix,&min_max_rows,&min_max_columns);
	for(c=0,i=1;c<min_max_columns-1;++c,++i)
	{
		min=dmlfw_mat_double_get(min_max_matrix,0,c);
		max=dmlfw_mat_double_get(min_max_matrix,1,c);
		value=dmlfw_row_vec_double_get(I,i);
		scaled_value=(value-min)/(max-min);
		dmlfw_row_vec_double_set(I,i,scaled_value);
	}

	
	m=dmlfw_column_vec_double_from_csv("model.csv");
	if(m==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_double_destroy(I);
		dmlfw_mat_double_destroy(min_max_matrix);
		return 0;
	}
	
	result=dmlfw_multiply_double_row_vector_with_column_vector(I,m);
	if(result==NULL)
	{
		printf("Low memory\n");
		dmlfw_row_vec_double_destroy(I);
		dmlfw_mat_double_destroy(min_max_matrix);
		dmlfw_column_vec_double_destroy(m);	
		return 0;
	}
	
	// now reverse the scaled the value
	scaled_value=dmlfw_column_vec_double_get(result,0);

	min=dmlfw_mat_double_get(min_max_matrix,0,min_max_columns-1);
	max=dmlfw_mat_double_get(min_max_matrix,1,min_max_columns-1);

	value=(scaled_value*(max-min))+min;

	printf("Prediction %lf\n",value);

	dmlfw_row_vec_double_destroy(I);
	dmlfw_mat_double_destroy(min_max_matrix);
	dmlfw_column_vec_double_destroy(m);	
	dmlfw_column_vec_double_destroy(result);
	return 0;
}
