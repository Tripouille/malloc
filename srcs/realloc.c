#include "ft_malloc.h"

size_t
min(size_t value1, size_t value2) {
	return (value1 < value2 ? value1 : value2);
}

void
memory_move(char *dst, char *src, size_t size) {
	size_t			i = (dst > src ? size - 1 : 0);

	while(size--)
	{
		dst[i] = src[i];
		i += (dst > src ? -1 : 1);
	}
}

void *
realloc(void *ptr, size_t size) {
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "calling realloc on %p and size %lu\n", ptr, size));


	t_zone_header *		zone = NULL;
	t_zone_header **	first_zone = NULL;

	if (ptr == NULL)
	{
		return (malloc(size));
	}
	else if (!size)
	{
		free(ptr);
		return (NULL);
	}
	//return (NULL);
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

	new_zone = malloc(size);
	if (new_zone == NULL)
		return (NULL);
	memmove(new_zone, ptr, min(size, block_manager->block_size));
	if (memcmp(new_zone, ptr, min(size, block_manager->block_size)))
		write(1, buffer, sprintf(buffer, "realloc de merde\n"));
	write(1, new_zone, min(size, block_manager->block_size));
	
	free(ptr);
	write(1, buffer, sprintf(buffer, "realloc return new_zone = %p\n", new_zone));
	return (new_zone);

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
	//}
	return (NULL);
}