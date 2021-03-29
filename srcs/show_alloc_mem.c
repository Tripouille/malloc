#include "memory_manager.h"

static size_t
print_block(t_block_manager * block_manager) {
	printptr(BLOCK_MANAGER_SHIFT(block_manager)); prints(" - "); printptr(NEXT_BLOCK_MANAGER(block_manager));
	prints(" : "); printnbase(block_manager->block_size, BASE10, 10); prints(" octets");
	newline();
	return (block_manager->block_size);
}

static size_t
show_zone(t_zone_header *zone_header) {
	size_t total = 0;

	for (t_zone_header * zone = zone_header; zone != NULL; zone = zone->next_zone_header)
		for (t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);
		ptr_is_in_zone(BLOCK_MANAGER_SHIFT(block_manager), zone);
		block_manager = NEXT_BLOCK_MANAGER(block_manager))
			if (!block_manager->is_free)
				total += print_block(block_manager);
	return (total);
}

static size_t
show_large() {
	size_t total = 0;

	for (t_zone_header * zone = g_memory_manager.large; zone != NULL; zone = zone->next_zone_header)
		total += print_block(ZONE_HEADER_SHIFT(zone));
	return (total);
}

void
show_alloc_mem(void) {
	size_t total = 0;

	pthread_mutex_lock(&g_memory_mutex);
	prints("TINY : "); printptr(g_memory_manager.tiny); newline();
	total += show_zone(g_memory_manager.tiny);
	prints("SMALL : "); printptr(g_memory_manager.small); newline();
	total += show_zone(g_memory_manager.small);
	prints("LARGE : "); printptr(g_memory_manager.large); newline();
	total += show_large();
	prints("Total : "); printnbase(total, BASE10, 10); prints(" octets"); newline();
	pthread_mutex_unlock(&g_memory_mutex);
}
