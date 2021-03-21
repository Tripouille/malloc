#include "memory_manager.h"

t_memory_manager memory_manager;
//char buffer[1000];

static size_t
calculate_padded_size(size_t size) {
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

static void *
get_mmap(size_t size) {
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
				block_manager = (void*)block_manager + sizeof(t_block_manager) + block_size;
				block_manager->block_size = remaining_size - sizeof(t_block_manager);
				block_manager->is_free = 1;
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
	t_zone_header ** zone_header = &memory_manager.large;

	while (*zone_header != NULL)
		zone_header = &(*zone_header)->next_zone_header;
	return ((*zone_header = get_new_zone(block_size)));
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
