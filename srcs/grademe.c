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
	for (int i = 0; i < 1; ++i)
	{
		addr = malloc(i + 1);
		//addr[0] = 1;
	}
	return (0);
}	