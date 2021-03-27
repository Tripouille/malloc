#include "ft_malloc.h"

int main(void)
{
	int i;
	i = 0;
	char * addr;
	while (i < 1024)
	{
		addr = (char*)malloc(1024); (void)addr;
		addr[0] = 42;
		free(addr);
		i++;
	}
	return (0);
}