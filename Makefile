CFILES = minishell.c utils.c itoa.c herdoc.c ft_split.c exectst.c parsing_utils.c parcing.c tools.c linked_list.c list_utils.c tokenization.c builtins_func.c execution.c free_resources.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address
CC = cc
GET = GET_NEXT_LINE/get_next_line.c  GET_NEXT_LINE/get_next_line_utils.c 
OGET = $(GET:.c=.o)
NAME = minishell
PRINTF = ft_printf/libftprintf.a
READLINE = -I/Users/haouky/.brew/opt/readline/include
all: $(NAME)

$(NAME) : $(OFILES) $(PRINTF) $(OGET)  minishell.h
		$(CC) $(CFLAGS) -lreadline /Users/arekoune/.brew/Cellar/readline/8.2.13/lib/libreadline.a $(OFILES) $(READLINE) $(PRINTF) $(OGET) -o $(NAME) 

$(PRINTF):
	make -C ft_printf
%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	make -C ft_printf fclean
	rm -rf $(OFILES)
	rm -rf $(OGET)
fclean: clean
	rm -rf $(NAME)
re: fclean all