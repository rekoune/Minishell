NAME	= libftprintf.a
SRCS   = ft_printf.c hex.c hexuper.c upnbr.c putads.c pnbr.c wrtc.c wrts.c
HEDER = ft_printf.h
OBJS	= ${SRCS:.c=.o}

CFLAGS	= -Wall -Werror -Wextra

all: $(NAME)

$(NAME): $(OBJS)
	ar rc $(NAME) $(OBJS)

%.o: %.c $(HEDER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all
