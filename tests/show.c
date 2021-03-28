#include "ft_malloc.h"

int main(void)
{
	void * p[10];

	p[1] = malloc(4096);
	show_alloc_mem_ex();
	p[7] = malloc(14097);
	show_alloc_mem_ex();
	p[7] = realloc(p[7], 15);
	show_alloc_mem_ex();

	p[7] = realloc(p[7], 44);
	show_alloc_mem_ex();

	p[7] = realloc(p[7], 1025);
	show_alloc_mem_ex();
	p[7] = realloc(p[7], 444444);
	show_alloc_mem_ex();
	p[8] = malloc(4444441);
	show_alloc_mem_ex();
	return (0);
}