#include "memory_manager.h"

t_memory_manager memory_manager;
char buffer[1000];
//write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));

size_t
calculate_padded_size(size_t size) {
	size_t page_size = getpagesize();
	return ((size / page_size) * page_size + (size % page_size ? page_size : 0));
}

void *
get_mmap(size_t size) {
	write(1, buffer, sprintf(buffer, "calling get_mmap for %li pages\n", size / getpagesize()));
	return (mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, 0, 0));
}

void *
get_new_zone(size_t size) {
	size_t const padded_size = calculate_padded_size(sizeof(t_zone_header) + size);
	void * new_zone = get_mmap(padded_size);

	if (new_zone == MAP_FAILED)
		return (NULL);
	((t_zone_header*)new_zone)->next_zone_header = NULL;
	((t_zone_header*)new_zone)->zone_size = padded_size - sizeof(t_zone_header);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->block_size = padded_size - sizeof(t_zone_header) - sizeof(t_block_manager);
	((t_block_manager*)(new_zone + sizeof(t_zone_header)))->is_free = 1;
	return (new_zone);
}



t_zone_header *
get_ptr_zone(void * ptr, t_zone_header *** first_zone) {
	void * start = NULL;
	void * end = NULL;

	for (t_zone_header * tiny = memory_manager.tiny; tiny != NULL; tiny = tiny->next_zone_header)
	{
		start = (void*)tiny + sizeof(t_zone_header) + sizeof(t_block_manager);
		end = start + tiny->zone_size;
		if (start <= ptr && ptr < end)
		{
			*first_zone = &memory_manager.tiny;
			return (tiny);
		}
	}
	for (t_zone_header * small = memory_manager.small; small != NULL; small = small->next_zone_header)
	{
		start = (void*)small + sizeof(t_zone_header) + sizeof(t_block_manager);
		end = start + small->zone_size;
		if (start <= ptr && ptr < end)
		{
			*first_zone = &memory_manager.small;
			return (small);
		}
	}
	for (t_zone_header * large = memory_manager.large; large != NULL; large = large->next_zone_header)
		if (ptr == (void*)large + sizeof(t_zone_header) + sizeof(t_block_manager))
		{
			*first_zone = &memory_manager.large;
			return (large);
		}
	return (NULL);
}