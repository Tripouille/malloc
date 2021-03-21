#ifndef MEMORY_MANAGER_H
# define MEMORY_MANAGER_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>

enum memory_settings {TINY = 1024, SMALL = 4096, BLOCK_PER_ZONE = 100};

typedef struct				s_block_manager
{
	unsigned int			block_size;
	char					is_free;
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

void * get_memory(size_t size);

#endif