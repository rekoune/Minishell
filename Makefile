CFILES = minishell.c list_utils.c parcing.c tools.c tokenization.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror
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