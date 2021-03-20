#include "malloc.h"

t_memory_manager memory_manager;
char buffer[1000];

static size_t calculate_padded_size(size_t size)
{
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

static void *get_memory(size_t size)
{
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0));
}

static void *get_new_zone(size_t size)
{
	size_t const padded_size = calculate_padded_size(sizeof(t_zone_header) + size);
	void * new_zone = get_memory(padded_size);
	write(1, buffer, sprintf(buffer, "get_new_zone with size %lu\n", padded_size));
	//----------------------------------------------------------------------
	if (new_zone == NULL)
		return (NULL);
	((t_zone_header*)new_zone)->next_zone_header = NULL;
	((t_zone_header*)new_zone)->zone_size = padded_size - sizeof(t_zone_header);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->is_free = 1;
	//----------------------------------------------------------------------
	write(1, buffer, sprintf(buffer, "new_zone->next_zone_header = %p\n", ((t_zone_header*)new_zone)->next_zone_header));
	write(1, buffer, sprintf(buffer, "new_zone->zone_size = %lu\n", ((t_zone_header*)new_zone)->zone_size));
	write(1, buffer, sprintf(buffer, "t_block_manager->block_size = %lu\n", ((t_block_manager*)(new_zone + sizeof(t_zone_header)))->block_size));
	write(1, buffer, sprintf(buffer, "t_block_manager->is_free = %i\n", ((t_block_manager*)(new_zone + sizeof(t_zone_header)))->is_free));
	return (new_zone);
}

static void *find_block_in_zone(size_t size, t_zone_header *zone_header)
{
	void *	zone_start = (void*)zone_header + sizeof(t_zone_header);
	void *	zone_end = zone_start + zone_header->zone_size;
	size_t	remaining_size = 0;

	for (t_block_manager *block_manager = zone_start; zone_start + sizeof(t_block_manager) < zone_end;
		zone_start += sizeof(t_block_manager) + block_manager->block_size)
	{
		write(1, buffer, sprintf(buffer, "find_block_in_zone zone_start = %p\n", zone_start));
		write(1, buffer, sprintf(buffer, "t_block_manager block_size = %lu\n", block_manager->block_size));
		write(1, buffer, sprintf(buffer, "t_block_manager is_free = %i\n", block_manager->is_free));
		if (block_manager->is_free && block_manager->block_size >= size)
		{
			remaining_size = block_manager->block_size - size;
			block_manager->block_size = size;
			block_manager->is_free = 0;
			if (remaining_size > sizeof(t_block_manager))
			{
				block_manager = zone_start + sizeof(t_block_manager) + size;
				block_manager->block_size = remaining_size - sizeof(t_block_manager);
				block_manager->is_free = 1;
			}
			return (zone_start + sizeof(t_block_manager));
		}
	}
	return (NULL);
}



static void * get_tiny_block(size_t size)
{
	write(1, buffer, sprintf(buffer, "starting get tiny chunk with size %lu\n", size));
	t_zone_header *actual_header = NULL; 
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
		chunk = find_block_in_zone(size, actual_header);
		write(1, buffer, sprintf(buffer, "find_block_in_zone ret = %p\n", chunk));
		if (chunk == NULL && actual_header->next_zone_header == NULL)
		{
			actual_header->next_zone_header = get_new_zone((TINY + sizeof(t_block_manager)) * BLOCK_PER_ZONE);
			if (actual_header->next_zone_header == NULL)
				return (NULL);
		}
		else if (chunk == NULL)
		{
			write(1, buffer, sprintf(buffer, "changing header to %p\n", actual_header->next_zone_header));
			actual_header = actual_header->next_zone_header;
		}
	} while (chunk == NULL);
	return (chunk);
}

static void * get_small_block(size_t size)
{
	(void)size;
	write(1, buffer, sprintf(buffer, "starting get small chunk with size %lu\n", size));
	if (memory_manager.small == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.small == NULL\n"));
		memory_manager.small = get_memory((SMALL + sizeof(t_block_manager)) * BLOCK_PER_ZONE);
	}
	return (NULL);
}

static void * get_large_block(size_t size)
{
	(void)size;
	write(1, buffer, sprintf(buffer, "starting get large chunk with size %lu\n", size));
	if (memory_manager.large == NULL)
	{
		write(1, buffer, sprintf(buffer, "memory_manager.large == NULL\n"));
		memory_manager.large = get_memory(size);
	}
	return (NULL);
}

void *malloc(size_t size)
{
	write(1, buffer, sprintf(buffer, "\nstarting malloc with size %lu\n", size));
	if (size <= TINY)
		return (get_tiny_block(size));
	else if (size <= SMALL)
		return (get_small_block(size));
	else
		return (get_large_block(size));
}