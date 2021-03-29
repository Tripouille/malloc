#include "ft_malloc.h"

static void
print_block_ex(t_block_manager * block_manager) {
	if (block_manager->is_free)
		set_color(GREEN);
	else
		set_color(RED);
	printnbase(block_manager->block_size, BASE10, 10);
}

static void
show_zone_ex(t_zone_header *zone_header) {
	size_t prev_page, actual_page, used_page;
	size_t page_size = getpagesize();

	for (t_zone_header * zone = zone_header; zone != NULL; zone = zone->next_zone_header) {
		prev_page = -1; used_page = 0;

		prints("[");
		for (t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);
		ptr_is_in_zone(BLOCK_MANAGER_SHIFT(block_manager), zone);
		block_manager = NEXT_BLOCK_MANAGER(block_manager)) {

			actual_page = (size_t)((void*)block_manager - ZONE_HEADER_SHIFT(zone)) / page_size;
			print_block_ex(block_manager); set_color(RESET);
			if (ptr_is_in_zone(NEXT_BLOCK_MANAGER(block_manager), zone))
				prints(" ");
			size_t	block_end_page = (NEXT_BLOCK_MANAGER(block_manager) - ZONE_HEADER_SHIFT(zone) - 1) / page_size;
			if (!block_manager->is_free && actual_page != prev_page) {
				used_page += div_up(block_manager->block_size + sizeof(t_block_manager), page_size);
				prev_page = actual_page;
			}
			else if (!block_manager->is_free && actual_page != block_end_page) {
				used_page += div_up(block_manager->block_size + sizeof(t_block_manager), page_size);
				prev_page = block_end_page;
			}
		}
		size_t	total_page = (zone->zone_size + sizeof(t_zone_header)) / page_size;
		prints("]("); printnbase(used_page, BASE10, 10); prints("/"); printnbase(total_page, BASE10, 10); prints(")");
		if (zone->next_zone_header != NULL)
			prints("->");
	}
}

void
show_alloc_mem_ex(void) {
	pthread_mutex_lock(&g_memory_mutex);
	prints("TINY->"); show_zone_ex(g_memory_manager.tiny); newline();
	prints("SMALL->"); show_zone_ex(g_memory_manager.small); newline();
	prints("LARGE->"); show_zone_ex(g_memory_manager.large); newline(); newline();
	pthread_mutex_unlock(&g_memory_mutex);
} 