#include "malloc.h"

void *malloc(size_t size)
{
	//write(1, buffer, sprintf(buffer, "\nstarting malloc with size %lu\n", size));
	if (size <= TINY)
		return (get_block_in_tiny_zone(size));
	else if (size <= SMALL)
		return (get_block_in_small_zone(size));
	else
		return (get_large_zone(size));
}