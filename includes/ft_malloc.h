#ifndef FT_MALLOC_H
# define FT_MALLOC_H
# include <string.h>
# include <limits.h>
# include <stdlib.h>
# include "memory_manager.h"
# include "utils.h"

void *		malloc(size_t size);
void *		salloc(size_t size, char value);
void *		calloc(size_t nmemb, size_t size);
void *		realloc(void *ptr, size_t size);
void		free(void * ptr);

void		show_alloc_mem(void);
void		show_alloc_mem_ex(void);

#endif