.DEFAULT_GOAL = launch
ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif
LINK	= libft_malloc
NAME	= $(LINK)_$(HOSTTYPE).so
SRCS	= $(addprefix srcs/, malloc.c)
OBJS	= $(SRCS:srcs/%.c=objs/%.o)
DEPS	= $(SRCS:srcs/%.c=deps/%.d)
WD		= $(shell pwd)

CC		= gcc
CFLAGS	= -Wall -Werror -Wextra -g3 -Iincludes
DFLAGS	= -MT $@ -MMD -MP -MF deps/$*.d

all: $(NAME)

$(OBJS): objs/%.o: srcs/%.c
	$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS)
	$(CC) -Wl,-soname,$(LINK) -shared $^ -o $(NAME)
	ln -fs $(NAME) $(LINK)

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(LINK) $(NAME) grademe

re: fclean all

grademe: srcs/grademe.c $(NAME)
	$(CC) -L$(WD) -Wl,-rpath=$(WD) $(CFLAGS) $< -o $@ -l:$(LINK)

launch: grademe
	./grademe

.PHONY: all clean fclean re launch
-include $(DEPS)