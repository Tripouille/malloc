#include "malloc.h"
//#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int ac, char ** av)
{
	(void)ac; (void)av;

	char * addr[10]; (void)addr;
	//for (int i = 0; i < -1; ++i) { addr = malloc(42); addr[0] = 1;}
	show_alloc_mem();
	addr[0] = malloc(42); addr[0][0] = 1;
	addr[1] = malloc(2000); addr[1][0] = 2;
	show_alloc_mem();
	free(addr[0]);
	free(addr[1]);
	show_alloc_mem();
	return (0);
}