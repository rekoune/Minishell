CFILES = minishell.c utils.c itoa.c herdoc.c ft_split.c exectst.c parsing_utils.c parcing.c tools.c linked_list.c list_utils.c tokenization.c builtins_func.c execution.c free_resources.c
OFILES = $(CFILES:.c=.o)

CFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address
CC = cc
GET = GET_NEXT_LINE/get_next_line.c GET_NEXT_LINE/get_next_line_utils.c 
OGET = $(GET:.c=.o)
NAME = minishell
PRINTF = ft_printf/libftprintf.a


# Get the Readline installation path
READLINE_DIR = $(shell brew --prefix readline)

# Set Readline flags
RFLAGS = -I$(READLINE_DIR)/include
LDFLAGS = -L$(READLINE_DIR)/lib -lreadline

all: $(NAME)

$(NAME): $(OFILES) $(PRINTF) $(OGET) minishell.h
	$(CC) $(CFLAGS) $(OFILES) $(PRINTF) $(OGET) -o $(NAME) $(LDFLAGS)

$(PRINTF):
	make -C ft_printf

%.o: %.c
	$(CC) $(CFLAGS) $(RFLAGS) -c $< -o $@

clean:
	make -C ft_printf fclean
	rm -rf $(OFILES)
	rm -rf $(OGET)

fclean: clean
	rm -rf $(NAME)

re: fclean all
