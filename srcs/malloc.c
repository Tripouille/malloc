#include "malloc.h"

void *malloc(size_t size)
{
	return (get_memory(size));
}