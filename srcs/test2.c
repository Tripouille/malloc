#include "ft_malloc.h"

int main(void)
{
	int i;
	char * addr; (void)addr;
	i = 0;
	while (i < 2)
	{
		addr = (char*)malloc(32411+i);
		addr[0] = 42;
		free(addr);
		i++;
	}
	show_alloc_mem();
	return (0);
}