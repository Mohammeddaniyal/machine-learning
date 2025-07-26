#include<stdio.h>
#include<daniyal.h>
#include<azka.h>
// because of the next 2 lines, memory is not going to get allocated for x and msg
extern __thread int x; // we are saying that x has been declared externally
extern __thread char msg[512]; // we are saying that msg has been declared externally
int main()
{
	daniyal("main");
	azka("main");
	printf("Address of global x is %p\n",&x);
	printf("Address of global msg is %p\n",&msg);
	return 0;	
}
