#include "utils.h"

size_t
min(size_t value1, size_t value2) {
	return (value1 < value2 ? value1 : value2);
}

void
memory_move(char *dst, char *src, size_t size) {
	if (dst > src)
		while (size--)
			dst[size] = src[size];
	else
		for (size_t i = 0; i < size; ++i)
			dst[i] = src[i];
}