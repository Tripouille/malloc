#include "ft_malloc.h"

void *
calloc(size_t nmemb, size_t size) {
	if (!nmemb || !size 
	|| nmemb > ULONG_MAX / size)
		return (NULL);

	return (salloc(nmemb * size, 0));
}
