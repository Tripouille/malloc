#include "memory_manager.h"

static size_t
show_alloc_tiny(char buffer[]) {
	size_t tiny_total = 0;

	write(1, buffer, sprintf(buffer, "TINY : %p\n", (void*)memory_manager.tiny));
	for (t_zone_header * tiny = memory_manager.tiny; tiny != NULL; tiny = tiny->next_zone_header)
		for (t_block_manager * block_manager = (void*)tiny + sizeof(t_zone_header);
		(size_t)((void*)tiny + sizeof(t_zone_header) + tiny->zone_size - (void*)block_manager) > sizeof(block_manager);
		block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
			if (!block_manager->is_free || 1) {
				write(1, buffer, sprintf(buffer, "%p - %p : %lu octets / free = %i\n", (void*)block_manager + sizeof(t_block_manager),
						(void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size, block_manager->is_free));
				tiny_total += block_manager->block_size;
			}
	return (tiny_total);
}

static size_t
show_alloc_small(char buffer[]) {
	size_t small_total = 0;

	write(1, buffer, sprintf(buffer, "SMALL : %p\n", (void*)memory_manager.small));
	for (t_zone_header * small = memory_manager.small; small != NULL; small = small->next_zone_header)
		for (t_block_manager * block_manager = (void*)small + sizeof(t_zone_header);
		(size_t)((void*)small + sizeof(t_zone_header) + small->zone_size - (void*)block_manager) > sizeof(block_manager);
		block_manager = (void*)block_manager + sizeof(t_block_manager) + block_manager->block_size)
			if (!block_manager->is_free || 1) {
				write(1, buffer, sprintf(buffer, "%p - %p : %lu octets / free = %i\n", (void*)block_manager + sizeof(t_block_manager),
						(void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size, block_manager->is_free));
				small_total += block_manager->block_size;
			}
	return (small_total);
}

static size_t
show_alloc_large(char buffer[]) {
	size_t              large_total = 0;
	t_block_manager *   block_manager = NULL;

	write(1, buffer, sprintf(buffer, "LARGE : %p\n", (void*)memory_manager.large));
	for (t_zone_header * large = memory_manager.large; large != NULL; large = large->next_zone_header)
	{
		block_manager = (void*)large + sizeof(t_zone_header);
		if (!block_manager->is_free || 1) {
			write(1, buffer, sprintf(buffer, "%p - %p : %lu octets / free = %i\n", (void*)block_manager + sizeof(t_block_manager),
					(void*)block_manager + sizeof(t_block_manager) + block_manager->block_size, block_manager->block_size, block_manager->is_free));
			large_total += block_manager->block_size;
		}
	}
	return (large_total);
}

void
show_alloc_mem(void) {
	char buffer[1000];
	size_t total = 0;
	//write(1, "\n", 1);
	total += show_alloc_tiny(buffer);
	total += show_alloc_small(buffer);
	total += show_alloc_large(buffer);
	write(1, buffer, sprintf(buffer, "Total : %lu octets\n\n", total));
}