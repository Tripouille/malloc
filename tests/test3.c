#include "ft_malloc.h"

int main(void)
{
	show_alloc_mem();
	void * p = malloc(33);
	p = malloc(1024);
	show_alloc_mem();
	free(p);
	show_alloc_mem();
	return (0);
}