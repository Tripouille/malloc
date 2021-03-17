#include "malloc.h"

void free(void *ptr)
{
	printf("starting free on %p\n", ptr);
}

void *malloc(size_t size)
{
	printf("starting malloc of size %lu\n", size);
	return (NULL);
}

void *realloc(void *ptr, size_t size)
{
	printf("starting realloc on %p  of size %lu\n", ptr,  size);
	return (NULL);
}

void coucou(void)
{
	printf("coucou\n");
}
