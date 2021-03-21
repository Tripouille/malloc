#include "malloc.h"

void *
malloc(size_t size) {
	return (get_memory(size));
}

void
free(void * ptr) {
    free_memory(ptr);
}