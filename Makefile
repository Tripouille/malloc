NAME = libmalloc.a
INCLUDES = $(addprefix includes/, malloc.h)
SRCS = $(addprefix srcs/, main.c)
OBJS = $(SRCS:srcs/%.c=objs/%.o)
DEPS = $(SRCS:srcs/%.c=deps/%.d)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iincludes
DFLAGS = -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) $< -o $@

$(NAME): $(OBJS)
	ar rcs $(NAME) $^

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
include $(wildcard, $(DEPS))