#include "ft_malloc.h"

void *
realloc(void *ptr, size_t size) {
	t_zone_header *		zone = NULL;
	t_zone_header **	first_zone = NULL;

	if (ptr == NULL)
		return (malloc(size));
	else if (!size)
	{
		free(ptr);
		return (NULL);
	}
	zone = get_ptr_zone(ptr, &first_zone);
	if (zone == NULL)
		return (NULL);
	t_block_manager *block_manager = get_block_manager(ptr, zone);
	if (block_manager == NULL)
		return (NULL);
	if (is_large_zone(zone) && zone->zone_size - sizeof(t_block_manager) >= size)
		return (ptr);
	//resize
	return (NULL);
}