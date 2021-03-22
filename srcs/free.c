#include "ft_malloc.h"

static bool
zone_is_completely_free(t_zone_header * zone) {
	t_block_manager * block_manager = (void*)zone + sizeof(t_zone_header);
	return (zone->zone_size == sizeof(t_block_manager) + block_manager->block_size
	&& block_manager->is_free);
}

static void
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
	else if (actual->next_zone_header != NULL)
	{
		*first_zone	= actual->next_zone_header;
		munmap(actual, sizeof(t_zone_header) + actual->zone_size);
	}
}

static void
defragller(t_zone_header * zone, t_zone_header ** first_zone) {
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
				block_manager->block_size = (void*)furthest_allocated_block_manager - ((void*)block_manager + sizeof(t_block_manager));
				break ;
			}
		}
	}
	if (zone_is_completely_free(zone))
		clean_memory_manager(first_zone);
}

static void
free_block(void * ptr, t_zone_header * ptr_zone, t_zone_header ** first_zone) {
	for (t_block_manager * block_manager = (void*)ptr_zone + sizeof(t_zone_header);
	(size_t)((void*)ptr_zone + sizeof(t_zone_header) + ptr_zone->zone_size - (void*)block_manager) > sizeof(t_block_manager);
	block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
		if ((void*)block_manager + sizeof(t_block_manager) == ptr) {
			block_manager->is_free = 1;
			return (defragller(ptr_zone, first_zone));
		}
}

void
free(void * ptr) {
	t_zone_header * ptr_zone = NULL;
	t_zone_header ** first_zone = NULL;

	if (ptr == NULL)
		return ;
	ptr_zone = get_ptr_zone(ptr, &first_zone);
	if (ptr_zone != NULL)
		free_block(ptr, ptr_zone, first_zone);
}
