#include "memory_manager.h"
# include "ft_malloc.h"

t_memory_manager memory_manager;
char buffer[10000];
//write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));

bool
zone_is_large(t_zone_header * zone) {
	return (zone->zone_size - sizeof(t_block_manager) > SMALL);
}

size_t
calculate_padded_size(size_t size) {
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

void *
get_mmap(size_t size) {
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0));
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

static bool
try_set_ptr_zone_info(void * ptr, t_zone_header * first_zone_header,
								t_ptr_infos *infos) {
	infos->prev_zone = NULL;

	for (infos->actual_zone = first_zone_header;
	infos->actual_zone != NULL; infos->actual_zone = infos->actual_zone->next_zone_header)
	{
		if (ptr >= ZONE_HEADER_SHIFT(infos->actual_zone)
		&& ptr < ZONE_HEADER_SHIFT(infos->actual_zone) + infos->actual_zone->zone_size)
			return (true);
		infos->prev_zone = infos->actual_zone;
	}
	return (false);
}

bool
set_ptr_zone_info(void * ptr, t_ptr_infos *infos) {
	if (try_set_ptr_zone_info(ptr, memory_manager.tiny, infos)
	|| try_set_ptr_zone_info(ptr, memory_manager.small, infos)
	|| try_set_ptr_zone_info(ptr, memory_manager.large, infos))
		return (true);
	return (false);
}

bool
set_ptr_info(void * ptr, t_ptr_infos *infos) {
	if (!set_ptr_zone_info(ptr, infos))
		return (false);
	
	void *	zone_end = ZONE_HEADER_SHIFT(infos->actual_zone) + infos->actual_zone->zone_size;
	for (infos->block_manager = ZONE_HEADER_SHIFT(infos->actual_zone);
	BLOCK_MANAGER_SHIFT(infos->block_manager) < zone_end;
	infos->block_manager = NEXT_BLOCK_MANAGER(infos->block_manager))
	{
		if (!infos->block_manager->is_free)
			infos->furthest_prev_allocated_block_manager = infos->block_manager ;
		if (BLOCK_MANAGER_SHIFT(infos->block_manager) == ptr)
			return (true);
	}
	return (false);
}