#ifndef MALLOC_H
# define MALLOC_H
# include "memory_manager.h"

void *		malloc(size_t size);
void		free(void * ptr);
void *		realloc(void *ptr, size_t size);

void		show_alloc_mem(void);

#endif