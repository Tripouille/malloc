#include "malloc.h"

/* void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
       int munmap(void *addr, size_t length);*/

int main(int ac, char ** av)
{
	printf("Start of main\n");
	(void)ac; (void)av;
	void * p = malloc(100000);
	(void)p; free(p);
	printf("End of main\n");
	return (0);
}