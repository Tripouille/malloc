.DEFAULT_GOAL = tests
TESTS 	= test0 test1 test2 test3 test4 test5

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
LINK	= libft_malloc.so
NAME	= $(LINK)_$(HOSTTYPE).so
SRCS	= $(addprefix srcs/, memory_manager.c show_alloc_mem.c show_alloc_mem_ex.c \
							utils.c malloc.c calloc.c realloc.c salloc.c free.c)
OBJS	= $(SRCS:srcs/%.c=objs/%.o)
DEPS	= $(SRCS:srcs/%.c=deps/%.d)
WD		= $(shell pwd)

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -g3 -Iincludes
DFLAGS	= -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) -fPIC $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) -shared $^ -o $(NAME)
	ln -fs $(NAME) $(LINK)

clean:
	rm -rf $(OBJS) $(DEPS) *.dSYM $(TESTS) show

fclean: clean
	rm -rf $(LINK) $(NAME) grademe
	rm -rf *.dSYM

re: fclean all

tests: $(TESTS)
$(TESTS) show: %: tests/%.c | $(NAME)
	@$(CC) -L$(WD) $(CFLAGS) $< -o $@ -lft_malloc
	@#./$@
	@#/bin/echo -n "$@:" && /usr/bin/time -v ./$@ 2>&1 | grep "Minor (reclaiming a frame) page faults:"
	@/bin/echo -n "$@:" && /usr/bin/time -l ./$@ 2>&1 | grep "page reclaims"
	@#/bin/echo -n "$@:" && /usr/bin/time -l ./$@ 2>&1

.PHONY: all clean fclean re launch $(TESTS) show
-include $(DEPS)