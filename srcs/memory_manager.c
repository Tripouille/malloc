#include "memory_manager.h"

t_memory_manager memory_manager;
char buffer[10000];
//write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));

size_t
calculate_padded_size(size_t size) {
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

void *
get_mmap(size_t size) {
	//write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));

	//char buffer[10000];
	void * result = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
	//if (result == MAP_FAILED)
	//	write(1, buffer, sprintf(buffer, "get_mmap FAIL!!!!!!!!!!!!!!!!!!!!!!!!!!!\n"));
	//else
		//write(1, buffer, sprintf(buffer, "get_mmap SUCCESS!!\n"));
	return (result);
}

void *
get_new_zone(size_t size) {
	size_t const padded_size = calculate_padded_size(sizeof(t_zone_header) + size);
	void * new_zone = get_mmap(padded_size);

	if (new_zone == MAP_FAILED)
		return (NULL);
	((t_zone_header*)new_zone)->next_zone_header = NULL;
	((t_zone_header*)new_zone)->zone_size = padded_size - sizeof(t_zone_header);
	((t_block_manager*)ZONE_HEADER_SHIFT(new_zone))->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	((t_block_manager*)ZONE_HEADER_SHIFT(new_zone))->is_free = 1;
	return (new_zone);
}

t_zone_header *
get_ptr_zone_in_specific_zone(void * ptr, t_zone_header *** first_zone, t_zone_header ** specific_zone) {
	void * start = NULL;
	void * end = NULL;
	for (t_zone_header * actual_zone = *specific_zone; actual_zone != NULL; actual_zone = actual_zone->next_zone_header)
	{
		start = ZONE_HEADER_SHIFT(actual_zone) + sizeof(t_block_manager);
		end = start + actual_zone->zone_size;
		if (start <= ptr && ptr < end)
		{
			write(1, buffer, sprintf(buffer, "ptr found in zone\n"));
			*first_zone = specific_zone;
			return (actual_zone);
		}
	}
	return (NULL);
}

t_zone_header *
get_ptr_zone(void * ptr, t_zone_header *** first_zone) {
	return(get_ptr_zone_in_specific_zone(ptr, first_zone, &memory_manager.tiny));
	/*t_zone_header *		zone = NULL;

	if ((zone = get_ptr_zone_in_specific_zone(ptr, first_zone, &memory_manager.tiny)) != NULL
	|| (zone = get_ptr_zone_in_specific_zone(ptr, first_zone, &memory_manager.small)) != NULL
	|| (zone = get_ptr_zone_in_specific_zone(ptr, first_zone, &memory_manager.large)) != NULL)
		return (zone);
	return (NULL);*/
}

bool
is_large_zone(t_zone_header * zone) {
	return (zone->zone_size - sizeof(t_block_manager) > SMALL);
}

t_block_manager *
get_block_manager(void * ptr, t_zone_header * zone) {

	void *	zone_end = ZONE_HEADER_SHIFT(zone) + zone->zone_size;


	for (t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);
	BLOCK_MANAGER_SHIFT(block_manager) < zone_end;
	block_manager = BLOCK_MANAGER_SHIFT(block_manager) + block_manager->block_size)
		if (BLOCK_MANAGER_SHIFT(block_manager) == ptr)
			return (block_manager);

	//char buffer[10000];
	//	write(1, buffer, sprintf(buffer, "get_block_manager return NULL\n"));
	write(1, buffer, sprintf(buffer, "get_block_manager ptr not found in zone\n"));

	return (NULL);
}