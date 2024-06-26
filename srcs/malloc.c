#include "ft_malloc.h"

static void *
find_block_in_zone(size_t block_size, t_zone_header *zone_header) {
	void *	zone_start = ZONE_HEADER_SHIFT(zone_header);
	void *	zone_end = zone_start + zone_header->zone_size;
	size_t	remaining_size = 0;

	for (t_block_manager *block_manager = zone_start;
	BLOCK_MANAGER_SHIFT(block_manager) < zone_end;) {
		if (block_manager->is_free && block_manager->block_size >= block_size + sizeof(t_block_manager)) {
			remaining_size = block_manager->block_size - block_size;
			block_manager->block_size = block_size;
			block_manager->is_free = 0;

			t_block_manager * next_block_manager = NEXT_BLOCK_MANAGER(block_manager);
			next_block_manager->block_size = remaining_size - sizeof(t_block_manager);
			next_block_manager->is_free = 1;
			return (BLOCK_MANAGER_SHIFT(block_manager));
		}
		else
			block_manager = BLOCK_MANAGER_SHIFT(block_manager) + block_manager->block_size;
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
		if (block == NULL)
			zone_header = &(*zone_header)->next_zone_header;
	} while (block == NULL);
	return (block);
}

static void *
get_block_in_tiny_zone(size_t block_size) {
	return (get_block_in_zone(block_size, (sizeof(t_block_manager) + TINY) * BLOCK_PER_ZONE, &g_memory_manager.tiny));
}

static void *
get_block_in_small_zone(size_t block_size) {
	return (get_block_in_zone(block_size, (sizeof(t_block_manager) + SMALL) * BLOCK_PER_ZONE, &g_memory_manager.small));
}

static void *
get_large_zone(size_t block_size) {
	size_t				const padded_size = calculate_padded_size(sizeof(t_zone_header) + sizeof(t_block_manager) + block_size);
	t_zone_header **	zone_header = &g_memory_manager.large;
	t_block_manager *	block_manager = NULL;

	while (*zone_header != NULL)
		zone_header = &(*zone_header)->next_zone_header;
	*zone_header = get_mmap(padded_size);
	if (*zone_header == MAP_FAILED)
		return (NULL);
	(*zone_header)->next_zone_header = NULL;
	(*zone_header)->zone_size = padded_size - sizeof(t_zone_header);
	block_manager = ZONE_HEADER_SHIFT(*zone_header);
	block_manager->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	block_manager->is_free = 0;
	return (BLOCK_MANAGER_SHIFT(block_manager));
}

static void *
get_memory(size_t size) {
	char const * mmap_threshold = getenv("M_MMAP_THRESHOLD");
	size_t value;

	if (mmap_threshold != NULL && set_size_t(mmap_threshold, &value)
	&& size > value)
		return (get_large_zone(size));
	else if (size <= TINY)
		return (get_block_in_tiny_zone(size));
	else if (size <= SMALL)
		return (get_block_in_small_zone(size));
	else
		return (get_large_zone(size));
}

void *
malloc(size_t size) {
	if (size && align_size(&size)) {
		pthread_mutex_lock(&g_memory_mutex);
		void *	ptr = get_memory(size);
		pthread_mutex_unlock(&g_memory_mutex);
		return (ptr);
	}
	return (NULL);
}
