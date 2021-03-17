ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
LINK = libft_malloc
NAME = $(LINK)_$(HOSTTYPE).so
SRCS = $(addprefix srcs/, malloc.c)
OBJS = $(SRCS:srcs/%.c=objs/%.o)
DEPS = $(SRCS:srcs/%.c=deps/%.d)

CC = gcc
CFLAGS = -Wall -Werror -Wextra -Iincludes
DFLAGS = -MT $@ -MMD -MP -MF deps/$*.d

all: grademe launch

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	gcc -shared $^ -o $(NAME)
	ln -s $(NAME) $(LINK)

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(LINK) $(NAME) grademe

re: fclean all

grademe: srcs/grademe.c $(NAME)
	$(CC) $(CFLAGS) -L. -l:$(LINK) $< -o $@ 

launch:
	@valgrind -q --leak-check=full ./grademe

.PHONY: all clean fclean re launch
include $(wildcard, $(DEPS))