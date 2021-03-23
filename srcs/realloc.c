#include "ft_malloc.h"

void
memcopy(char *src, char *dest, size_t size) {
	while(size--)
		dest[size] = src[size];
}

void *
realloc(void *ptr, size_t size) {
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "calling realloc on %p and size %lu\n", ptr, size));


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
	//if (is_large_zone(zone) && zone->zone_size - sizeof(t_block_manager) >= size
	//|| (size < block_manager->block_size && block_manager->block_size - size <= sizeof(t_block_manager)))
		//return (ptr);
	void * new_zone = NULL;
	/*if (is_large_zone(zone))
	{
		new_zone = malloc(size);
		if (new_zone == NULL)
			return (NULL);
		memcopy(new_zone, ptr, block_manager->block_size);
		free(ptr);
		return (new_zone);
	}
	else*/
	//{
		new_zone = malloc(size);
		if (new_zone == NULL)
			return (NULL);
		memcopy(new_zone, ptr, block_manager->block_size);
		free(ptr);
		return (new_zone);
	//}
	return (NULL);
}