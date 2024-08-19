CFILES = minishell.c utils.c ft_split.c parsing_utils.c parcing.c tools.c linked_list.c list_utils.c tokenization.c builtins_func.c excu.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
CC = cc

NAME = minishell

all: $(NAME)

$(NAME) : $(OFILES) minishell.h
		$(CC) $(CFLAGS) $(OFILES) -o $(NAME) -lreadline

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OFILES)
fclean: clean
	rm -rf $(NAME)
re: fclean all