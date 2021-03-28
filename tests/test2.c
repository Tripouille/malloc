#include "ft_malloc.h"

int main(void)
{
	//int i;
	//i = 0;
	char * addr; (void)addr;
	addr = malloc(2032);
		show_alloc_mem_ex();
	addr = malloc(234234234);
		show_alloc_mem_ex();
	

	/*while (i < 1024)
	{
		addr = (char*)malloc(1024);
		addr[0] = 42;
		//show_alloc_mem_ex();
		free(addr);
		i++;
	}*/
	return (0);
}