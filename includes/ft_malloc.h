#ifndef FT_MALLOC_H
# define FT_MALLOC_H
# include <string.h>
# include <limits.h>
# include "memory_manager.h"
# include "utils.h"

void *		malloc(size_t size);
void 		*calloc(size_t nmemb, size_t size);
void		free(void * ptr);
void *		realloc(void *ptr, size_t size);

void		show_alloc_mem(void);
void		show_alloc_mem_ex(void);

#endif