#ifndef MALLOC_H
# define MALLOC_H
# include <stdio.h>
# include <unistd.h>
# include <sys/mman.h>

/* void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
       int munmap(void *addr, size_t length);*/

void free(void *ptr);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);

#endif