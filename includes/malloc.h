#ifndef MALLOC_H
# define MALLOC_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>

enum zone_type {TINY, SMALL, LARGE};

typedef struct s_malloc_zone
{
	enum zone_type			type;
	void					*start;
	size_t					size;
	struct s_malloc_zone	*next_zone;
}				t_malloc_zone;

void *malloc(size_t size);

#endif