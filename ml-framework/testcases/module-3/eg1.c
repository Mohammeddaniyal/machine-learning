#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int main()
{
	int x;
	double value;
	double min,max;
	srand(time(NULL));
	min=-100.0;
	max=100;
	x=1;
	while(x<=10)
	{
		value=rand();
		value= (value*(max-min)/RAND_MAX)+min;
		printf("%lf\n",value);
		x++;
	}
	return 0;
}
