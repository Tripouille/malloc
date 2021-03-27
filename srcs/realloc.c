#include "ft_malloc.h"

void *
realloc(void *ptr, size_t size) {
	//char buffer[10000];
	//write(1, buffer, sprintf(buffer, "calling realloc on %p and size %lu\n", ptr, size));
	t_ptr_infos infos;

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
	if (!set_ptr_info(ptr, &infos))
		return (NULL);
	//if (zone_is_large(zone) && zone->zone_size - sizeof(t_block_manager) >= size
	//|| (size < block_manager->block_size && block_manager->block_size - size <= sizeof(t_block_manager)))
		//return (ptr);
	void * new_zone = NULL;

	new_zone = malloc(size);
	if (new_zone == NULL)
		return (NULL);
	memmove(new_zone, ptr, min(size, infos.block_manager->block_size));
	//if (memcmp(new_zone, ptr, min(size, block_manager->block_size)))
		//write(1, buffer, sprintf(buffer, "realloc de merde\n"));
	//write(1, buffer, sprintf(buffer, "avant le free\n"));
	free(ptr);
	//write(1, buffer, sprintf(buffer, "realloc return new_zone = %p\n", new_zone));
	return (new_zone);

	/*if (zone_is_large(zone))
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