#include "ft_malloc.h"

int main(void)
{
	//int i;
	char * addr; (void)addr;
	/*i = 0;
	while (i < 2)
	{
		addr = (char*)malloc(32411+i);
		addr[0] = 42;
		free(addr);
		i++;
	}*/
	addr = malloc(11);
	strcpy(addr, "0123456789");
	printf("%s\n", addr);
	addr = realloc(addr, 10000);
	printf("%s\n", addr);
	addr = realloc(addr, 2);
	addr[1] = 0;
	printf("%s\n", addr);
	return (0);
}