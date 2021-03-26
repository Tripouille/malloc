#ifndef MEMORY_MANAGER_H
# define MEMORY_MANAGER_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>

#define ZONE_HEADER_SHIFT(z) ((void*)z + sizeof(t_zone_header))
#define BLOCK_MANAGER_SHIFT(bm) ((void*)bm + sizeof(t_block_manager))
#define NEXT_BLOCK_MANAGER(bm) (BLOCK_MANAGER_SHIFT(bm) + bm->block_size)

enum memory_settings {TINY = 40960, SMALL = 4096, BLOCK_PER_ZONE = 100};

typedef struct				s_block_manager
{
	size_t			block_size;
	size_t			is_free;
} 							t_block_manager;

typedef struct	s_zone_header
{
	struct s_zone_header			*next_zone_header;
	size_t							zone_size;
}				t_zone_header;

typedef struct s_memory_manager
{
	t_zone_header	*tiny;
	t_zone_header	*small;
	t_zone_header	*large;
}				t_memory_manager;

typedef struct s_ptr_infos
{
	t_zone_header	*prev_zone;
	t_zone_header	**actual_zone;
	t_block_manager *block_manager;
	t_block_manager *furthest_prev_allocated_block_manager;
}				t_ptr_infos;

extern t_memory_manager memory_manager;
extern char buffer[10000];

bool				zone_is_large(t_zone_header * zone);
size_t				calculate_padded_size(size_t size);
void *				get_mmap(size_t size);
void *				get_new_zone(size_t size);
bool				set_ptr_info(void * ptr, t_ptr_infos *infos);

#endif