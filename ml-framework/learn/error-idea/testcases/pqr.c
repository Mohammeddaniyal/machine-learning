#include<stdio.h>
#include<daniyal.h>
#include<azka.h>
#include<pthread.h>
// because of the next 2 lines, memory is not going to get allocated for x and msg
extern int x; // we are saying that x has been declared externally
extern char msg[512]; // we are saying that msg has been declared externally
		  
void * func_1(void *)
{
	daniyal("func_1");
	azka("func_1");
	return NULL;
}

void * func_2(void *)
{	
	daniyal("func_2");
	azka("func_2");
	return NULL;
}

int main()
{
	daniyal("main");
	azka("main");
	printf("Address of global x is %p\n",&x);
	printf("Address of global msg is %p\n",&msg);
	return 0;	
}
