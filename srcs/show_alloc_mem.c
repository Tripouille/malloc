#include "memory_manager.h"

static size_t
print_block(t_block_manager * block_manager, int fd) {
	printptr(BLOCK_MANAGER_SHIFT(block_manager), fd); prints(" - ", fd); printptr(NEXT_BLOCK_MANAGER(block_manager), fd);
	prints(" : ", fd); printnbase(block_manager->block_size, "0123456789", 10, fd); prints(" octets", fd);
	newline(fd);
	return (block_manager->block_size);
}

size_t
show_zone(t_zone_header *zone_header) {
	size_t total = 0;

	for (t_zone_header * zone = zone_header; zone != NULL; zone = zone->next_zone_header)
		for (t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);
		ptr_is_in_zone(BLOCK_MANAGER_SHIFT(block_manager), zone);
		block_manager = NEXT_BLOCK_MANAGER(block_manager))
			if (!block_manager->is_free)
				total += print_block(block_manager, 1);
	return (total);
}

size_t
show_large() {
	size_t total = 0;

	for (t_zone_header * zone = g_memory_manager.large; zone != NULL; zone = zone->next_zone_header)
		total += print_block(ZONE_HEADER_SHIFT(zone), 1);
	return (total);
}

void
show_alloc_mem(void) {
	size_t total = 0;

	prints("TINY : ", 1); printptr(g_memory_manager.tiny, 1); newline(1);
	total += show_zone(g_memory_manager.tiny);
	prints("SMALL : ", 1); printptr(g_memory_manager.small, 1); newline(1);
	total += show_zone(g_memory_manager.small);
	prints("LARGE : ", 1); printptr(g_memory_manager.large, 1); newline(1);
	total += show_large();
	prints("Total : ", 1); printnbase(total, "0123456789", 10, 1); prints(" octets", 1); newline(1);
}