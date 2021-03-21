#include "malloc.h"
//#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
       int munmap(void *addr, size_t length);*/

int main(int ac, char ** av)
{
	(void)ac; (void)av;

	char * addr; (void)addr;
	for (int i = 0; i < -1; ++i) { addr = malloc(42); addr[0] = 1;}
	addr = malloc(1); addr[0] = 1;
	char * addr2 = malloc(30);
	free(addr2);
	show_alloc_mem();
	return (0);
}