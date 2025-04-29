NAME			=	minishell
SRCS	= 	main.c check.c path_find.c builtin.c


OBJS			=	$(SRCS:.c=.o)
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
CC 				= 	cc
LIBFT_DIR		= 	./libft
LIBFT 			= 	$(LIBFT_DIR)/libft.a
READLINE_FLAGS 	= 	$(shell pkg-config --cflags --libs readline)


all: $(LIBFT) $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

$(LIBFT):
		$(MAKE) -C $(LIBFT_DIR)

clean:
	$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re

