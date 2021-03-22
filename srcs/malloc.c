#include "malloc.h"

void *
malloc(size_t size) {
	return (get_memory(size));
}

void
free(void * ptr) {
    free_memory(ptr);
}

void *
realloc(void *ptr, size_t size) {
    char buffer[100000];
	write(1, buffer, sprintf(buffer, "SMALL : %p\n", (void*)memory_manager.small));

    (void)ptr, (void)size;
    return (NULL);
}