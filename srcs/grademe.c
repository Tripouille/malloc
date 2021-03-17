#include "malloc.h"
#include <unistd.h>

/* void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
       int munmap(void *addr, size_t length);*/

int main(int ac, char ** av)
{
	(void)ac; (void)av;
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "Start of main\n"));
	void * p = malloc(100000);
	free(p);
	write(1, buffer, sprintf(buffer, "End of main\n"));
	return (0);
}