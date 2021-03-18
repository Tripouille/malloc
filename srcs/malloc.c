#include "malloc.h"

static void *get_memory(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0));
}

static t_malloc_zone *get_malloc_zone(void)
{
	static t_malloc_zone malloc_zone;
	return (&malloc_zone);
}

void *malloc(size_t size)
{
	t_malloc_zone * malloc_zone = get_malloc_zone(); (void)malloc_zone;
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "starting malloc of size %lu\n",size));
	return (get_memory(size));
}