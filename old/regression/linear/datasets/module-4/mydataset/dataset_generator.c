#include<stdio.h>
#include<stdlib.h>
#include<math.h>
int main(int argc,char *argv[])
{
	double x,y;
	char *target;
	int start_x,end_x;
	FILE *file;
	int j;
	if(argc!=4)
	{
		printf("Specify [target.csv] [start_x] [end_x]\n");
		return 0;
	}
	target=argv[1];
	file=fopen(target,"w");
	fprintf(file,"0,1\n"); // header part
	start_x=atoi(argv[2]);
	end_x=atoi(argv[3]);
	for(j=start_x;j<=end_x;++j)
	{
		x=(double)j;
		y=pow(sin(1.0/15.0*x),2)*x+10;
		fprintf(file,"%lf,%lf\n",x,y);
	}
	fclose(file);
	return 0;
}
