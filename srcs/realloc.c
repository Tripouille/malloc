#include "ft_malloc.h"

static void *
move_into_new_block(t_ptr_infos *infos, size_t size) {
	pthread_mutex_unlock(&g_memory_mutex);
	void *		new_ptr = NULL;
	void *		actual_ptr = BLOCK_MANAGER_SHIFT(infos->block_manager);

	if ((new_ptr = malloc(size)) == NULL)
		return (NULL);
	memory_move(new_ptr, actual_ptr, min(size, infos->block_manager->block_size));
	free(actual_ptr);
	return (new_ptr);
}

static void *
extend_memory(t_ptr_infos *infos, size_t size) {
	t_block_manager *	next_block_manager = NEXT_BLOCK_MANAGER(infos->block_manager);

	if (!ptr_is_in_zone(BLOCK_MANAGER_SHIFT(next_block_manager), *infos->actual_zone))
		return (move_into_new_block(infos, size));
	size_t	total_size = infos->block_manager->block_size + next_block_manager->block_size;
	if (next_block_manager->is_free && total_size >= size)
	{
		infos->block_manager->block_size = size;
		((t_block_manager*)NEXT_BLOCK_MANAGER(infos->block_manager))->block_size = total_size - size;
		((t_block_manager*)NEXT_BLOCK_MANAGER(infos->block_manager))->is_free = 1;
		pthread_mutex_unlock(&g_memory_mutex);
		return (BLOCK_MANAGER_SHIFT(infos->block_manager));
	}
	else
		return (move_into_new_block(infos, size));
}

static void
split_block(t_ptr_infos * infos, size_t size) {
	size_t	available_size = infos->block_manager->block_size - size - sizeof(t_block_manager);
	infos->block_manager->block_size = size;
	t_block_manager * new_block_manager = NEXT_BLOCK_MANAGER(infos->block_manager);
	new_block_manager->block_size = available_size;
	new_block_manager->is_free = 1;
}

void *
realloc(void *ptr, size_t size) {
	t_ptr_infos infos;

	if (ptr == NULL)
		return (malloc(size));
	else if (!size)
		free(ptr);
	if (!size || !align_size(&size))
		return (NULL);

	pthread_mutex_lock(&g_memory_mutex);
	if (!set_ptr_info(ptr, &infos)) {
		pthread_mutex_unlock(&g_memory_mutex);
		return (NULL);
	}

	bool	is_large_zone = zone_is_large(*infos.actual_zone);
	if (is_large_zone && size <= SMALL)
		return (move_into_new_block(&infos, size));
	else if (infos.block_manager->block_size >= size)
	{
		if (!is_large_zone && infos.block_manager->block_size - size > sizeof(t_block_manager))
			split_block(&infos, size);
		pthread_mutex_unlock(&g_memory_mutex);
		return (ptr);
	}
	else if (is_large_zone)
		return (move_into_new_block(&infos, size));
	else
		return (extend_memory(&infos, size));
}