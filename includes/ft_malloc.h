#ifndef FT_MALLOC_H
# define FT_MALLOC_H
# include "memory_manager.h"
# include <string.h>
# include <limits.h>

void *		malloc(size_t size);
void 		*calloc(size_t nmemb, size_t size);
void		free(void * ptr);
void *		realloc(void *ptr, size_t size);

void		show_alloc_mem(void);

#endif