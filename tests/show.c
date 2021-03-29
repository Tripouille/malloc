#include "ft_malloc.h"

int main(void)
{
	void * p[10]; (void)p;
	show_alloc_mem_ex(); newline();
	p[0] = malloc(16);
	p[1] = malloc(32);
	p[2] = malloc(64);
	show_alloc_mem_ex(); newline();
	free(p[1]);
	show_alloc_mem_ex(); newline();
	free(p[0]);
	show_alloc_mem_ex(); newline();
	free(p[2]);
	show_alloc_mem_ex(); newline();
	p[0] = malloc(2032);
	p[1] = malloc(2032);
	show_alloc_mem_ex(); newline();
	p[1] = realloc(p[1], 2033);
	show_alloc_mem_ex(); newline();
	p[2] = malloc(TINY + 64);
	show_alloc_mem_ex(); newline();
	p[1] = realloc(p[1], TINY);
	show_alloc_mem_ex(); newline();
	free(p[1]);
	show_alloc_mem_ex(); newline();
	p[3] = malloc(SMALL * 10);
	show_alloc_mem_ex(); newline();
	p[3] = realloc(p[3], SMALL + 1);
	show_alloc_mem_ex(); newline();
	p[3] = realloc(p[3], SMALL);
	show_alloc_mem_ex(); newline();
	return (0);
}