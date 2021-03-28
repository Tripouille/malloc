#ifndef UTILS_H
# define UTILS_H
# include <stddef.h>
# include <stdbool.h>
# include <unistd.h>

#define BASE10 "0123456789"
#define BASE16 "0123456789ABCDEF"
#define GREEN "\e[0;32m"
#define RED "\e[0;31m"
#define RESET "\e[0;0m"

size_t	min(size_t value1, size_t value2);
void	memory_move(char * dst, char * src, size_t size);
bool	align_size(size_t * size);
size_t	slen(char const * s);
void	prints(char const * s);
void	printnbase(size_t n, char const * base, size_t base_size);
void	printptr(void * ptr);
void	newline();
void	set_color(char const * color);

#endif