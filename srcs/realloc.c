#include "ft_malloc.h"

void *
realloc(void *ptr, size_t size) {
	if (ptr == NULL)
		return (malloc(size));
	else if (!size)
	{
		free(ptr);
		return (NULL);
	}
	t_block_manager *ptr_manager = get_block_manager(ptr);
	if (ptr_manager == NULL)
		return (NULL);
	return (NULL);
}