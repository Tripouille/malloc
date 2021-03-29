#include "ft_malloc.h"

#define M (1024 * 1024)

int main(void)
{
	char *addr;

	addr = malloc(16);
	free(NULL);
	free((void*)addr + 5);
	if (realloc((void*) + 5, 10) == NULL)
		prints("Bonjours\n");
	return (0);
}