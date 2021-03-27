#include "utils.h"

size_t
min(size_t value1, size_t value2) {
	return (value1 < value2 ? value1 : value2);
}

void
memory_move(char * dst, char * src, size_t size) {
	if (dst > src)
		while (size--)
			dst[size] = src[size];
	else
		for (size_t i = 0; i < size; ++i)
			dst[i] = src[i];
}

bool
align_size(size_t *size) {
	if ((*size & 15) == 0)
		return (true);
	if (*size + 15 < *size)
		return (false);
	*size = (*size + 15) & ~15;
	return (true);
}

size_t
slen(char const * s) {
	size_t	size = 0;

	while (s[size])
		++size;
	return (size);
}

void
prints(char const * s, int fd) {
	write(fd, s, slen(s));
}

void
printnbase(size_t n, char const * base, size_t base_size, int fd) {
	if (n >= base_size)
		printnbase(n / base_size, base, base_size, fd);
	write(fd, base + n % base_size, 1);
}

void
printptr(void * ptr, int fd) {
	prints("0x", 1);
	printnbase((size_t)ptr, "0123456789ABCDEF", 16, fd);
}

void
newline(int fd) {
	write(fd, "\n", 1);
}