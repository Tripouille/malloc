#include "memory_manager.h"
# include "ft_malloc.h"

t_memory_manager	g_memory_manager;
pthread_mutex_t		g_memory_mutex = PTHREAD_MUTEX_INITIALIZER;


bool
zone_is_large(t_zone_header * zone) {
	for (t_zone_header * actual_zone = g_memory_manager.large;
	actual_zone != NULL; actual_zone = actual_zone->next_zone_header)
		if (actual_zone == zone)
			return (true); 
	return (false);
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
	size_t		padded_size = calculate_padded_size(sizeof(t_zone_header) + size);
	void *		new_zone = get_mmap(padded_size);

	if (new_zone == MAP_FAILED)
		return (NULL);
	((t_zone_header*)new_zone)->next_zone_header = NULL;
	((t_zone_header*)new_zone)->zone_size = padded_size - sizeof(t_zone_header);
	((t_block_manager*)ZONE_HEADER_SHIFT(new_zone))->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	((t_block_manager*)ZONE_HEADER_SHIFT(new_zone))->is_free = 1;
	return (new_zone);
}

bool
ptr_is_in_zone(void * ptr, t_zone_header *zone_header) {
	return (ptr >= ZONE_HEADER_SHIFT(zone_header)
	&& ptr < ZONE_HEADER_SHIFT(zone_header) + zone_header->zone_size);

}

static bool
try_set_ptr_zone_info(void * ptr, t_zone_header ** first_zone_header,
								t_ptr_infos *infos) {
	infos->prev_zone = NULL;

	for (infos->actual_zone = first_zone_header;
	*infos->actual_zone != NULL; infos->actual_zone = &(*infos->actual_zone)->next_zone_header)
	{
		if (ptr_is_in_zone(ptr, *infos->actual_zone))
			return (true);
		infos->prev_zone = *infos->actual_zone;
	}
	return (false);
}

static bool
set_ptr_zone_info(void * ptr, t_ptr_infos *infos) {
	if (try_set_ptr_zone_info(ptr, &g_memory_manager.tiny, infos)
	|| try_set_ptr_zone_info(ptr, &g_memory_manager.small, infos)
	|| try_set_ptr_zone_info(ptr, &g_memory_manager.large, infos))
		return (true);
	return (false);
}

bool
set_ptr_info(void * ptr, t_ptr_infos *infos) {
	if (!set_ptr_zone_info(ptr, infos))
		return (false);

	infos->closest_prev_allocated_block_manager = NULL;
	void *	zone_end = ZONE_HEADER_SHIFT(*infos->actual_zone) + (*infos->actual_zone)->zone_size;
	for (infos->block_manager = ZONE_HEADER_SHIFT(*infos->actual_zone);
	BLOCK_MANAGER_SHIFT(infos->block_manager) < zone_end;
	infos->block_manager = NEXT_BLOCK_MANAGER(infos->block_manager))
	{
		if (BLOCK_MANAGER_SHIFT(infos->block_manager) == ptr)
			return (true);
		if (!infos->block_manager->is_free)
			infos->closest_prev_allocated_block_manager = infos->block_manager;
	}
	return (false);
}