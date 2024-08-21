CFILES = minishell.c utils.c herdoc.c ft_split.c parsing_utils.c parcing.c tools.c linked_list.c list_utils.c tokenization.c builtins_func.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror -g -fsanitize=address
CC = cc
GET = GET_NEXT_LINE/get_next_line.c  GET_NEXT_LINE/get_next_line_utils.c 
OGET = $(GET:.c=.o)
NAME = minishell

all: $(NAME)

$(NAME) : $(OFILES)  $(OGET)  minishell.h
		$(CC) $(CFLAGS) $(OFILES)  $(OGET) -o $(NAME) -lreadline

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OFILES)
	rm -rf $(OGET)
fclean: clean
	rm -rf $(NAME)
re: fclean all