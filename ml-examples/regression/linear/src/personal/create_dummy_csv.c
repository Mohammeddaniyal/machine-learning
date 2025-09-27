#include<stdio.h>
#include<stdlib.h>
int main()
{
	double a;
	FILE *t;
	t=fopen("test.csv","w");
	fprintf(t,"col-1,col-2\n");
	a=1.0;
	while(a<=1947)
	{
		fprintf(t,"%lf,%lf\n",a,a);
		++a;
	}
	fclose(t);
	return 0;
}
/** @} */
