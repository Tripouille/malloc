#include "ft_malloc.h"

#define M (1024 * 1024)

int main(void)
{
	void * p; (void)p;
	p = malloc(1024);
	p = malloc(1024 * 32);
	p = malloc(1024 * 1024);
	p = malloc(1024 * 1024 * 16);
	p = malloc(1024 * 1024 * 128);
	show_alloc_mem();
	return (0);
}