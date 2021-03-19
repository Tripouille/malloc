#include "malloc.h"

t_memory_manager memory_manager;
char buffer[1000];

static size_t calculate_padded_size(size_t size)
{
	size_t const page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

static void *get_memory(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0));
}

static void *get_new_zone(size_t size)
{
	size_t const padded_size = calculate_padded_size(size);
	void * new_zone = get_memory(padded_size);
	if (new_zone == NULL)
		return (NULL);
	((size_t*)(new_zone))[0] = padded_size - MANAGER_RSIZE;
	((char*)(new_zone))[padded_size - 1] = FREE;
	return (new_zone);
}

static void * get_tiny_chunk(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get tiny chunk with size %lu\n", size));
	if (memory_manager.tiny == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.tiny == NULL\n"));
		memory_manager.tiny = get_new_zone((TINY + MANAGER_RSIZE) * BLOCKS_PER_ZONE);
	}
	return (NULL);
}

static void * get_small_chunk(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get small chunk with size %lu\n", size));
	if (memory_manager.small == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.small == NULL\n"));
		memory_manager.small = get_memory((SMALL + MANAGER_RSIZE) * BLOCKS_PER_ZONE);
	}
	return (NULL);
}

static void * get_large_chunk(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get large chunk with size %lu\n", size));
	if (memory_manager.large == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.large == NULL\n"));
		memory_manager.large = get_memory(size);
	}
	return (NULL);
}

static void * get_chunk(size_t size)
{
	if (size <= TINY)
		return (get_tiny_chunk(size));
	else if (size <= SMALL)
		return (get_small_chunk(size));
	else
		return (get_large_chunk(size));
}

void *malloc(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting malloc of size %lu\n",size));
	//----------------------------------------------------------------------
	void * chunk = get_chunk(size);
	//----------------------------------------------------------------------
	write(1, buffer, sprintf(buffer, "endind malloc chunk ptr =  %p\n", chunk));
	return (chunk);
}