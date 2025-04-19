SRCS	= 	main.c		\


OBJS			=	$(SRCS:.c=.o)
RM				=	rm -f
CFLAGS			=	-Wall -Wextra -Werror
CC 				= 	cc
LIBFT_DIR		= 	libft
LIBFT 			= 	$(LIBFT_DIR)/libft.a
READLINE_FLAGS 	= 	$(shell pkg-config --cflags --libs readline)
NAME			=	minishell

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(OBJS) $(LIBFT) $(READLINE_FLAGS) -o $(NAME)

clean:
	$(RM) $(OBJS)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re