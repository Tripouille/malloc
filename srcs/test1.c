#include "ft_malloc.h"

int main(void)
{
	int i;
	char * addr; (void)addr;
	i = 0;
	while (i < 1024)
	{
		addr = (char*)realloc(NULL, 1024);
		addr[0] = 42;
		i++;
	}
	return (0);
}