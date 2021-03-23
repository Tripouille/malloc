#include "malloc.h"
//#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{

	char * addr[1000]; (void)addr; int i = 0;
	//for (int i = 0; i < -1; ++i) { addr = malloc(42); addr[0] = 1;}
	addr[i++] = malloc(2000000); 
	//addr[i++] = malloc(2);
	//addr[i++] = malloc(3000);
	//show_alloc_mem();

	free(addr[0]);
	//free(addr[1]);
	//show_alloc_mem();
	//free(addr[2]);
	return (0);
}