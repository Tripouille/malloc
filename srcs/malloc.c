#include "ft_malloc.h"

static void *
find_block_in_zone(size_t block_size, t_zone_header *zone_header) {
	void *	zone_start = (void*)zone_header + sizeof(t_zone_header);
	void *	zone_end = zone_start + zone_header->zone_size;
	size_t	remaining_size = 0;

	for (t_block_manager *block_manager = zone_start; (size_t)(zone_end - (void*)block_manager) > sizeof(t_block_manager);) {
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
	void *		block = NULL;

	do {
		if (*zone_header == NULL && (*zone_header = get_new_zone(zone_size)) == NULL)
			return (NULL);
		block = find_block_in_zone(block_size, *zone_header);
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
	if (*zone_header == MAP_FAILED)
		return (NULL);

	(*zone_header)->next_zone_header = NULL;
	(*zone_header)->zone_size = padded_size - sizeof(t_zone_header);
	block_manager = (void*)*zone_header + sizeof(t_zone_header);
	block_manager->block_size = block_size;
	block_manager->is_free = 0;
	return ((void*)block_manager + sizeof(t_block_manager));
}

void *
get_memory(size_t size) {
	if (size <= TINY)
		return (get_block_in_tiny_zone(size));
	else if (size <= SMALL)
		return (get_block_in_small_zone(size));
	else
		return (get_large_zone(size));
}

void *
malloc(size_t size) {
	if (size)
		return (get_memory(size));
	return (NULL);
}
