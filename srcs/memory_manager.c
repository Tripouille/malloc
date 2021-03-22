#include "memory_manager.h"

t_memory_manager memory_manager;
char buffer[1000];

static size_t
calculate_padded_size(size_t size) {
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

static void *
get_mmap(size_t size) {
	write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0));
}

static void *
get_new_zone(size_t size) {
	size_t const padded_size = calculate_padded_size(sizeof(t_zone_header) + size);
	void * new_zone = get_mmap(padded_size);
	//write(1, buffer, sprintf(buffer, "get_new_zone with size %lu\n", padded_size));
	if (new_zone == NULL)
		return (NULL);
	((t_zone_header*)new_zone)->next_zone_header = NULL;
	((t_zone_header*)new_zone)->zone_size = padded_size - sizeof(t_zone_header);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->is_free = 1;
	//write(1, buffer, sprintf(buffer, "new_zone->next_zone_header = %p\n", ((t_zone_header*)new_zone)->next_zone_header));
	//write(1, buffer, sprintf(buffer, "new_zone->zone_size = %lu\n", ((t_zone_header*)new_zone)->zone_size));
	//write(1, buffer, sprintf(buffer, "t_block_manager->block_size = %lu\n", ((t_block_manager*)(new_zone + sizeof(t_zone_header)))->block_size));
	//write(1, buffer, sprintf(buffer, "t_block_manager->is_free = %i\n", ((t_block_manager*)(new_zone + sizeof(t_zone_header)))->is_free));
	return (new_zone);
}

static void *
find_block_in_zone(size_t block_size, t_zone_header *zone_header) {
	void *	zone_start = (void*)zone_header + sizeof(t_zone_header);
	void *	zone_end = zone_start + zone_header->zone_size;
	size_t	remaining_size = 0;

	for (t_block_manager *block_manager = zone_start; (size_t)(zone_end - (void*)block_manager) > sizeof(t_block_manager);) {
		//write(1, buffer, sprintf(buffer, "find_block_in_zone zone_start = %p\n", zone_start));
		//write(1, buffer, sprintf(buffer, "t_block_manager block_size = %lu\n", block_manager->block_size));
		//write(1, buffer, sprintf(buffer, "t_block_manager is_free = %i\n", block_manager->is_free));
		if (block_manager->is_free && block_manager->block_size >= block_size) {
			remaining_size = block_manager->block_size - block_size;
			block_manager->block_size = block_size;
			block_manager->is_free = 0;
			if (remaining_size > sizeof(t_block_manager)) {
				t_block_manager * next_block_manager = (void*)block_manager + sizeof(t_block_manager) + block_size;
				next_block_manager->block_size = remaining_size - sizeof(t_block_manager);
				next_block_manager->is_free = 1;
			}
			return ((void*)block_manager + sizeof(t_block_manager));
		}
		else
			block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size;
	}
	return (NULL);
}

static void *
get_block_in_zone(size_t block_size, size_t zone_size, t_zone_header ** zone_header) {
	//write(1, buffer, sprintf(buffer, "starting get tiny block with size %lu\n", block_size));
	void *		block = NULL;

	do {
		//write(1, buffer, sprintf(buffer, "actual header = %p\n", *zone_header));
		if (*zone_header == NULL && (*zone_header = get_new_zone(zone_size)) == NULL)
			return (NULL);
		block = find_block_in_zone(block_size, *zone_header);
		//if (block == NULL)
			//write(1, buffer, sprintf(buffer, "going to next header\n"));
		zone_header = &(*zone_header)->next_zone_header;
	} while (block == NULL);
	return (block);
}

static void *
get_block_in_tiny_zone(size_t block_size) {
	return (get_block_in_zone(block_size, (sizeof(t_block_manager) + TINY) * BLOCK_PER_ZONE, &memory_manager.tiny));
}

static void *
get_block_in_small_zone(size_t block_size) {
	return (get_block_in_zone(block_size, (sizeof(t_block_manager) + SMALL) * BLOCK_PER_ZONE, &memory_manager.small));
}

static void *
get_large_zone(size_t block_size) {
	size_t				const padded_size = calculate_padded_size(sizeof(t_zone_header) + sizeof(t_block_manager) + block_size);
	t_zone_header **	zone_header = &memory_manager.large;
	t_block_manager *	block_manager = NULL;

	while (*zone_header != NULL)
		zone_header = &(*zone_header)->next_zone_header;
	*zone_header =  get_mmap(padded_size);
	if (*zone_header == NULL)
		return (NULL);

	(*zone_header)->next_zone_header = NULL;
	(*zone_header)->zone_size = padded_size - sizeof(t_zone_header);
	block_manager = (void*)*zone_header + sizeof(t_zone_header);
	block_manager->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	block_manager->is_free = 0;
	return ((void*)block_manager + sizeof(t_block_manager));
}

void *
get_memory(size_t size) {
	//write(1, buffer, sprintf(buffer, "calling malloc of size %lu\n", size));
	if (size <= TINY)
		return (get_block_in_tiny_zone(size));
	else if (size <= SMALL)
		return (get_block_in_small_zone(size));
	else
		return (get_large_zone(size));
}

/*---------------------------------------------------------------------------------------------------------------*/

static t_zone_header *
get_ptr_zone(void * ptr, t_zone_header *** first_zone) {
	void * start = NULL;
	void * end = NULL;

	for (t_zone_header * tiny = memory_manager.tiny; tiny != NULL; tiny = tiny->next_zone_header)
	{
		start = (void*)tiny + sizeof(t_zone_header) + sizeof(t_block_manager);
		end = start + tiny->zone_size;
		if (start <= ptr && ptr < end)
		{
			*first_zone = &memory_manager.tiny;
			return (tiny);
		}
	}
	for (t_zone_header * small = memory_manager.small; small != NULL; small = small->next_zone_header)
	{
		start = (void*)small + sizeof(t_zone_header) + sizeof(t_block_manager);
		end = start + small->zone_size;
		if (start <= ptr && ptr < end)
		{
			*first_zone = &memory_manager.small;
			return (small);
		}
	}
	for (t_zone_header * large = memory_manager.large; large != NULL; large = large->next_zone_header)
		if (ptr == (void*)large + sizeof(t_zone_header) + sizeof(t_block_manager))
		{
			*first_zone = &memory_manager.large;
			return (large);
		}
	return (NULL);
}

static bool
zone_is_completely_free(t_zone_header * zone) {
	t_block_manager * block_manager = (void*)zone + sizeof(t_zone_header);
	return (zone->zone_size == sizeof(t_block_manager) + block_manager->block_size
	&& block_manager->is_free);
}

static void
clean_memory_manager(t_zone_header ** first_zone) {
	//write(1, buffer, sprintf(buffer, "calling clean_memory_manager\n"));
	t_zone_header * actual = NULL;
	t_zone_header * prev = NULL;
	for (actual = *first_zone;
	actual != NULL && !zone_is_completely_free(actual);
	actual = actual->next_zone_header)
		prev = actual;
	//write(1, buffer, sprintf(buffer, "clean_memory_manager post for\n"));
	if (prev != NULL)
	{
		prev->next_zone_header = actual->next_zone_header;
		munmap(actual, sizeof(t_zone_header) + actual->zone_size);
	}
	else if (actual->next_zone_header != NULL)
	{
		*first_zone	= actual->next_zone_header;
		munmap(actual, sizeof(t_zone_header) + actual->zone_size);
	}
}

static void
defragller(t_zone_header * zone, t_zone_header ** first_zone) {
	//write(1, buffer, sprintf(buffer, "starting defragller\n"));
	for (t_block_manager * block_manager = (void*)zone + sizeof(t_zone_header);
	(size_t)((void*)zone + sizeof(t_zone_header) + zone->zone_size - (void*)block_manager) > sizeof(t_block_manager);
	block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size) {
		if (block_manager->is_free) {
			t_block_manager * next_block_manager = (void*)block_manager + sizeof(t_block_manager)
													+ block_manager->block_size;
			t_block_manager * furthest_allocated_block_manager;

			for (furthest_allocated_block_manager = next_block_manager;
			(size_t)((void*)zone + sizeof(t_zone_header) + zone->zone_size
			- (void*)furthest_allocated_block_manager) > sizeof(t_block_manager) && furthest_allocated_block_manager->is_free;
			furthest_allocated_block_manager = (void*)furthest_allocated_block_manager + sizeof(t_block_manager)
												+ furthest_allocated_block_manager->block_size);

			if (next_block_manager != furthest_allocated_block_manager) {
				//write(1, buffer, sprintf(buffer, "degraglling the zone %p\n", (void*)zone));
				block_manager->block_size = (void*)furthest_allocated_block_manager - ((void*)block_manager + sizeof(t_block_manager));
				break ;
			}
		}
	}
	//write(1, buffer, sprintf(buffer, "fin defragller\n"));
	if (zone_is_completely_free(zone))
		clean_memory_manager(first_zone);
	//else
		//write(1, buffer, sprintf(buffer, "zone_is_ NOT completely_free\n"));
}

static void
free_block(void * ptr, t_zone_header * ptr_zone, t_zone_header ** first_zone) {
	//write(1, buffer, sprintf(buffer, "block in range free block\n"));
	for (t_block_manager * block_manager = (void*)ptr_zone + sizeof(t_zone_header);
	(size_t)((void*)ptr_zone + sizeof(t_zone_header) + ptr_zone->zone_size - (void*)block_manager) > sizeof(t_block_manager);
	block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
		if ((void*)block_manager + sizeof(t_block_manager) == ptr) {
			block_manager->is_free = 1;
			return (defragller(ptr_zone, first_zone));
		}
}

void
free_memory(void * ptr) {
	t_zone_header * ptr_zone = NULL;
	t_zone_header ** first_zone = NULL;

	//write(1, buffer, sprintf(buffer, "calling free on %p\n", ptr));
	ptr_zone = get_ptr_zone(ptr, &first_zone);
	if (ptr_zone != NULL)
		free_block(ptr, ptr_zone, first_zone);
}