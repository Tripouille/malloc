#include "utils.h"


size_t
div_up(size_t value, size_t div) {
	return ((value / div) + (value % div ? 1 : 0));
}

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
prints(char const * s) {
	write(1, s, slen(s));
}

void
printnbase(size_t n, char const * base, size_t base_size) {
	if (n >= base_size)
		printnbase(n / base_size, base, base_size);
	write(1, base + n % base_size, 1);
}

void
printptr(void * ptr) {
	prints("0x");
	printnbase((size_t)ptr, BASE16, 16);
}

void
newline(void) {
	write(1, "\n", 1);
}

void
set_color(char const * color) {
	prints(color);
}

bool	is_digit(char c) {
	return (c >= '0' && c <= '9');
}

bool	set_size_t(const char *s, size_t *n)
{
	*n = 0;
	while (is_digit(*s) && *n * 10 + *s - '0' > *n)
		*n = *n * 10 + *s++ - '0';
	return (!*s);
}