#include "ft_malloc.h"

static void
free_all_zone(t_zone_header * zone_header) {
	if (zone_header == NULL)
		return ;
	free_all_zone(zone_header->next_zone_header);
	munmap(zone_header, sizeof(t_zone_header) + zone_header->zone_size);
}


void free_all(void)
{
	pthread_mutex_lock(&g_memory_mutex);
	free_all_zone(g_memory_manager.tiny); g_memory_manager.tiny = NULL;
	free_all_zone(g_memory_manager.small); g_memory_manager.small = NULL;
	free_all_zone(g_memory_manager.large); g_memory_manager.large = NULL;
	pthread_mutex_unlock(&g_memory_mutex);
}
