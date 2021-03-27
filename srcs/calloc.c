#include "ft_malloc.h"
#include <strings.h>

void *
calloc(size_t nmemb, size_t size) {
	if (nmemb > ULONG_MAX / size)
		return (NULL);

	size_t	required_size = nmemb * size;
	char * ptr = (char*)malloc(required_size);

	if (ptr == NULL)
		return (NULL);
	while (required_size--)
		ptr[required_size] = 0;
	return (ptr);
}
