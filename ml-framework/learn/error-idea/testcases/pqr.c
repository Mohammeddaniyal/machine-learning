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
	void *ptr;
	pthread_t t1,t2;
	pthread_create(&t1,NULL,func_1,NULL);
	pthread_create(&t2,NULL,func_2,NULL);	
	daniyal("main");
	azka("main");
	printf("main - Address of global x is %p\n",&x);
	printf("main - Address of global msg is %p\n",&msg);
	pthread_join(t1,&ptr);
// because of the next 2 lines, memory is not going to get allocated for x and msg
extern int x; // we are saying that x has been declared externally
extern char msg[512]; // we are saying that msg has been declared externally
	pthread_join(t2,&ptr);
	return 0;	
}
