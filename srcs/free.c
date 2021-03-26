#include "ft_malloc.h"
/*
static bool
zone_is_completely_free(t_zone_header * zone) {
	t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);

	if (zone_is_large(zone) && block_manager->is_free)
		return (true);
	return (zone->zone_size == sizeof(t_block_manager) + block_manager->block_size
	&& block_manager->is_free);
}*/

/*static void
clean_memory_manager(t_zone_header ** first_zone) {
	t_zone_header * actual = NULL;
	t_zone_header * prev = NULL;

	for (actual = *first_zone;
	actual != NULL && !zone_is_completely_free(actual);
	actual = actual->next_zone_header)
		prev = actual;
	if (prev != NULL)
	{
		prev->next_zone_header = actual->next_zone_header;
		munmap(actual, sizeof(t_zone_header) + actual->zone_size);
	}
	else if (zone_is_large(actual) || actual->next_zone_header != NULL)
	{
		*first_zone	= actual->next_zone_header;
		munmap(actual, sizeof(t_zone_header) + actual->zone_size);
	}
}*/

static t_block_manager *
get_furthest_next_allocated_block_manager(t_block_manager * release_block_manager, t_zone_header * actual_zone) {
	void *	zone_end = ZONE_HEADER_SHIFT(actual_zone) + actual_zone->zone_size;
	t_block_manager * actual_block_manager = NULL;

	for (actual_block_manager = release_block_manager;
	BLOCK_MANAGER_SHIFT(actual_block_manager) < zone_end && actual_block_manager->is_free;
	actual_block_manager = NEXT_BLOCK_MANAGER(actual_block_manager));

	return (actual_block_manager);
/*
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
				block_manager->block_size = (void*)furthest_allocated_block_manager - ((void*)block_manager + sizeof(t_block_manager));
				break ;
			}
		}
	}*/
	
}

static void
defragller(t_block_manager * release_block_manager, t_zone_header * actual_zone) {
	t_block_manager * furthest_next_allocated_block_manager = get_furthest_next_allocated_block_manager(release_block_manager, actual_zone);
	(void)furthest_next_allocated_block_manager;

}

static void
free_block(t_ptr_infos * infos) {

	infos->block_manager->is_free = 1;
	defragller(infos->block_manager, infos->actual_zone);
	//if (zone_is_completely_free(infos->actual_zone))
		//clean_memory_manager(infos);
}

void
free(void * ptr) {
	t_ptr_infos infos;

	if (ptr != NULL && set_ptr_info(ptr, &infos))
		free_block(&infos);
}
