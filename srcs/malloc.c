#include "malloc.h"
#include <unistd.h>

void free(void *ptr)
{
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "starting free on %p\n", ptr));
}

void *malloc(size_t size)
{
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "starting malloc of size %lu\n",size));
	return (NULL);
}

void *realloc(void *ptr, size_t size)
{
	char buffer[10000];
	write(1, buffer, sprintf(buffer, "starting realloc on %p of size %lu\n", ptr, size));
	return (NULL);
}