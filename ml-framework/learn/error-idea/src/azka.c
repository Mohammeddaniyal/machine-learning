#include<stdio.h>
#include<daniyal.h>
// because of the next 2 lines, memory is not going to get allocated for x and msg
extern int x; // we are saying that x has been declared externally
extern char msg[512]; // we are saying that msg has been declared externally
void azka()
{
	printf("azka - address of global x is %p\n",&x);
	printf("azka - address of global msg is %p\n",&msg);
}
