#include<stdio.h>
#include<azka.h>
// because of the next 2 lines, memory is not going to get allocated for x and msg
extern __thread int x; // we are saying that x has been declared externally
extern __thread char msg[512]; // we are saying that msg has been declared externally
void azka(char *thread)
{
	printf("%s - azka - address of global x is %p\n",thread,&x);
	printf("%s - azka - address of global msg is %p\n",thread,&msg);
}
