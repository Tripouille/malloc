#ifndef MEMORY_MANAGER_H
# define MEMORY_MANAGER_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>
# include <stdbool.h>

enum memory_settings {TINY = 1024, SMALL = 4096, BLOCK_PER_ZONE = 100};

typedef struct				s_block_manager
{
	size_t			block_size;
	char			is_free;
} __attribute__((packed))	t_block_manager;

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

extern t_memory_manager memory_manager;

size_t			calculate_padded_size(size_t size);
void *			get_mmap(size_t size);
void *			get_new_zone(size_t size);
t_zone_header *	get_ptr_zone(void * ptr, t_zone_header *** first_zone);

#endif