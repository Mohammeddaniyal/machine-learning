#include<stdio.h>
void doSomething(int x)
{
	printf("%d\n",x*x);
}
void sam(void)
{
	printf("Cool\n");
}
void tom()
{
	printf("Nice\n");
}
int main()
{
	doSomething(10);
	tom("How",20); // compiler will ignore this args and not raise any error
	sam(10,20,"GOOD"); // will raise error error becauase of void
	return 0; 
}
