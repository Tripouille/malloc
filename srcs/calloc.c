#include "ft_malloc.h"
#include <strings.h>

void *calloc(size_t nmemb, size_t size) {
	if (nmemb * size / size != nmemb)
		return (NULL);
	void * ptr = malloc(nmemb * size);

	if (ptr == NULL)
		return (NULL);
	bzero(ptr, nmemb * size);
	return (ptr);
}