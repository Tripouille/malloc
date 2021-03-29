#include "ft_malloc.h"

void *
salloc(size_t size, char value) {
	char * ptr = (char*)malloc(size);

	if (ptr == NULL)
		return (NULL);
	while (size--)
		ptr[size] = value;
	return (ptr);
}
