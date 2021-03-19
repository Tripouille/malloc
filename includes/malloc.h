#ifndef MALLOC_H
# define MALLOC_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>

enum settings {TINY = 1024, SMALL = 4096, MANAGER_RSIZE = 9, BLOCKS_PER_ZONE = 1};
enum status {FREE, ALLOCATED};

typedef struct s_malloc_zone
{
	void					*start;
	size_t					size;
	struct s_malloc_zone	*next_zone;
}				t_malloc_zone;

typedef struct s_memory_manager
{
	t_malloc_zone	*tiny;
	t_malloc_zone	*small;
	t_malloc_zone	*large;
}				t_memory_manager;
void *malloc(size_t size);

extern t_memory_manager memory_manager;

#endif