#include "ft_malloc.h"

static void *
move_into_new_block(t_ptr_infos *infos, size_t size) {
	void *		new_ptr = NULL;
	void *		actual_ptr = BLOCK_MANAGER_SHIFT(infos->block_manager);

	if ((new_ptr = malloc(size)) == NULL)
		return (NULL);
	memmove(new_ptr, actual_ptr, min(size, infos->block_manager->block_size));
	free(actual_ptr);
	return (new_ptr);
}

static void *
extend_memory(t_ptr_infos *infos, size_t size) {
	if (zone_is_large(*infos->actual_zone))
		return (move_into_new_block(infos, size));

	t_block_manager *	next_block_manager = NEXT_BLOCK_MANAGER(infos->block_manager);

	if (!ptr_is_in_zone(BLOCK_MANAGER_SHIFT(next_block_manager), *infos->actual_zone))
		return (move_into_new_block(infos, size));
	
	size_t	total_size = infos->block_manager->block_size + next_block_manager->block_size;
	if (next_block_manager->is_free && total_size > size)
	{
		infos->block_manager->block_size = size;
		((t_block_manager*)NEXT_BLOCK_MANAGER(infos->block_manager))->block_size = total_size - size;
		return (BLOCK_MANAGER_SHIFT(infos->block_manager));
	}
	else
		return (move_into_new_block(infos, size));
}

void *
realloc(void *ptr, size_t size) {
	t_ptr_infos infos;

	if (ptr == NULL)
		return (malloc(size));
	else if (!size)
	{
		free(ptr);
		return (NULL);
	}
	else if (!set_ptr_info(ptr, &infos))
		return (NULL);

	size = (size + 15) & ~15;
	if (infos.block_manager->block_size >= size)
	{
		write(1, buffer, sprintf(buffer, "realloc bloc size already fit\n"));
		return (ptr);
	}
	else
		return (extend_memory(&infos, size));
}