#include "ft_malloc.h"
/*
static bool
zone_is_completely_free(t_zone_header * zone) {
	t_block_manager * block_manager = (void*)zone + sizeof(t_zone_header);
	if (is_large_zone(zone) && block_manager->is_free)
		return (true);
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
	else if (is_large_zone(actual) || actual->next_zone_header != NULL)
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
}*/

static void
free_block(void * ptr, t_zone_header * ptr_zone, t_zone_header ** first_zone) {
	t_block_manager * block_manager = get_block_manager(ptr, ptr_zone);

	if (block_manager == NULL)
		return ;
	block_manager->is_free = 1;
	//defragller(ptr_zone, first_zone);
	(void)first_zone;
}

void
free(void * ptr) {
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "calling free on %p\n", ptr));
	t_zone_header * ptr_zone = NULL;
	t_zone_header ** first_zone = NULL;

	if (ptr == NULL)
		return ;
	ptr_zone = get_ptr_zone(ptr, &first_zone);
	if (ptr_zone != NULL)
		free_block(ptr, ptr_zone, first_zone);
	//show_alloc_mem();
}
