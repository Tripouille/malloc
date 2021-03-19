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
	size_t const padded_size = calculate_padded_size(sizeof(t_header) + size);
	void * new_zone = get_memory(padded_size);
	write(1, buffer, sprintf(buffer, "get_new_zone with size %lu\n", padded_size));
	//----------------------------------------------------------------------
	if (new_zone == NULL)
		return (NULL);
	((t_header*)new_zone)->next_header = NULL;
	((t_header*)new_zone)->zone_size = padded_size - sizeof(t_header);
	((t_block_manager*)(new_zone + sizeof(t_header)))->block_size = padded_size - sizeof(t_header) - sizeof(t_block_manager);
	((t_block_manager*)(new_zone + sizeof(t_header)))->is_free = 1;
	//----------------------------------------------------------------------
	write(1, buffer, sprintf(buffer, "new_zone->next_header = %p\n", ((t_header*)new_zone)->next_header));
	write(1, buffer, sprintf(buffer, "new_zone->zone_size = %lu\n", ((t_header*)new_zone)->zone_size));
	write(1, buffer, sprintf(buffer, "t_block_manager->block_size = %lu\n", ((t_block_manager*)(new_zone + sizeof(t_header)))->block_size));
	write(1, buffer, sprintf(buffer, "t_block_manager->is_free = %i\n", ((t_block_manager*)(new_zone + sizeof(t_header)))->is_free));
	return (new_zone);
}

static void *find_chunk(size_t size, t_header *header)
{
	char *start = (char*)header + sizeof(t_header);
	char *end = start + header->zone_size;
	void *chunk = NULL;
	size_t remaining_size;
	while (start < end)
	{
		if (((t_block_manager*)start)->is_free && ((t_block_manager*)start)->block_size >= size)
		{
			((t_block_manager*)start)->is_free = 0;
			remaining_size = ((t_block_manager*)start)->block_size - size;
			if (remaining_size > sizeof(t_block_manager))
			{
				((t_block_manager*)(start + sizeof(t_block_manager) + size))->block_size = remaining_size - sizeof(t_block_manager);
				((t_block_manager*)(start + sizeof(t_block_manager) + size))->is_free = 1;
			}
			return (start + sizeof(t_block_manager));
		}
		start += sizeof(t_block_manager) + ((t_block_manager*)start)->block_size;
	}
	return (chunk);
}

static void * get_tiny_chunk(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get tiny chunk with size %lu\n", size));
	t_header *actual_header = NULL; 
	void * chunk = NULL;
	
	if (memory_manager.tiny == NULL)
	{
		memory_manager.tiny = get_new_zone((TINY + sizeof(t_block_manager)) * BLOCK_PER_ZONE);
		if (memory_manager.tiny == NULL)
			return (NULL);
	}
	actual_header = memory_manager.tiny;
	write(1, buffer, sprintf(buffer, "starting do while finding\n"));
	do
	{
		write(1, buffer, sprintf(buffer, "actual header = %p\n", actual_header));
		chunk = find_chunk(size, actual_header);
		if (chunk == NULL && actual_header->next_header == NULL)
		{
			actual_header->next_header = get_new_zone((TINY + sizeof(t_block_manager)) * BLOCK_PER_ZONE);
			if (actual_header->next_header == NULL)
				return (NULL);
		}
		else
			actual_header = actual_header->next_header;
	} while (chunk == NULL);
	return (chunk);
}

static void * get_small_chunk(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get small chunk with size %lu\n", size));
	if (memory_manager.small == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.small == NULL\n"));
		memory_manager.small = get_memory((SMALL + sizeof(t_block_manager)) * BLOCK_PER_ZONE);
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
	write(1, buffer, sprintf(buffer, "\nstarting malloc of size %lu\n",size));
	//----------------------------------------------------------------------
	void * chunk = get_chunk(size);
	//----------------------------------------------------------------------
	write(1, buffer, sprintf(buffer, "endind malloc chunk ptr =  %p\n", chunk));
	return (chunk);
}