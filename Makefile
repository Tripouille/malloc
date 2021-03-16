NAME = libmalloc.a
SRCS = $(addprefix srcs/, malloc.c)
OBJS = $(SRCS:srcs/%.c=objs/%.o)
DEPS = $(SRCS:srcs/%.c=deps/%.d)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iincludes
DFLAGS = -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	ar rcs $(NAME) $^

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(NAME) grademe

re: fclean all

grademe: srcs/grademe.c $(NAME)
	$(CC) $(CFLAGS) -L. -lmalloc $< -o $@

.PHONY: all clean fclean re
include $(wildcard, $(DEPS))