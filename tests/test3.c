#include "ft_malloc.h"

int main(void)
{
	show_alloc_mem_ex();
	void * p = malloc(33);
	p = malloc(1024);
	show_alloc_mem_ex();
	free(p);
	show_alloc_mem_ex();
	return (0);
}