#include "malloc.h"

int main(void)
{
	int i;
	char * addr; (void)addr;
	i = 0;
	while (i < 1024)
	{
		addr = (char*)malloc(1024);
		addr[0] = 42;
		free(addr);
		i++;
	}
	show_alloc_mem();
	return (0);
}