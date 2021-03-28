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

	prints("TINY : "); printptr(g_memory_manager.tiny); newline();
	total += show_zone(g_memory_manager.tiny);
	prints("SMALL : "); printptr(g_memory_manager.small); newline();
	total += show_zone(g_memory_manager.small);
	prints("LARGE : "); printptr(g_memory_manager.large); newline();
	total += show_large();
	prints("Total : "); printnbase(total, BASE10, 10); prints(" octets"); newline();
}



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
	for (t_zone_header * zone = zone_header; zone != NULL; zone = zone->next_zone_header)
	{
		prints("[");
		for (t_block_manager * block_manager = ZONE_HEADER_SHIFT(zone);
		ptr_is_in_zone(BLOCK_MANAGER_SHIFT(block_manager), zone);
		block_manager = NEXT_BLOCK_MANAGER(block_manager))
		{
			print_block_ex(block_manager);
			if (ptr_is_in_zone(NEXT_BLOCK_MANAGER(block_manager), zone))
				prints(" ");
			set_color(RESET);
		}
		prints("]");
		if (zone->next_zone_header != NULL)
			prints("->");
	}
}

void
show_alloc_mem_ex(void) {
	prints("TINY->"); show_zone_ex(g_memory_manager.tiny); newline();
	prints("SMALL->"); show_zone_ex(g_memory_manager.small); newline();
	prints("LARGE->"); show_zone_ex(g_memory_manager.large); newline(); newline();
} 