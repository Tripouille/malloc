#include "ft_malloc.h"

#define M (1024 * 1024)

int main(void)
{
	char *addr1;
	char *addr2;
	char *addr3;

	addr1 = (char*)malloc(16 * M);
	strcpy(addr1, "Bonjours\n");
	prints(addr1);
	addr2 = (char*)malloc(16*M); (void)addr2;
	addr3 = (char*)realloc(addr1, 128 * M);
	addr3[127*M] = 42;
	prints(addr3);
	return (0);
}