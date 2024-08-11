#ifndef MINISHELL_H
#define MINISHELL_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_cmd_list
{
	char **command;
	void *next;
}t_cmd_list;


enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	NEW_LINE = '\n',
	QOUTE = '\'',
	DOUBLE_QUOTE = '\"',
	ESCAPE = '\\',
	ENV = '$',
	PIPE_LINE = '|',
	REDIR_IN = '<',
	REDIR_OUT = '>',
	HERE_DOC,
	DREDIR_OUT,
};

enum	e_state
{
	GENERAL,
	IN_QUOTE,
	IN_DQUOTE,
};

typedef struct	s_lexer_list
{
	char			    *content;
	int				    len;
	enum e_token		type;
	enum e_state		state;
	struct	s_lexer_list		 *next;
	struct	s_lexer_list		  *prev;
}t_lexer_list;
//minishell.c

//list_utils.c
t_lexer_list	*new_node(char *str);
void		add_back(t_lexer_list **head, t_lexer_list *new);

//ft_split.c
char		**ft_split(char *str, char separator);
int 		word_counter(char *str, char	separator);
char		*str_dup(char *str, int size);
int			str_len(char *str, char separator);

//parcing.c
char 		**parcing(char *str);
t_cmd_list	*get_commands(char *str);

//tools.c
char	*str_ncopy(char *str, int size);
int		check_char(char c);
int		str_comp(char *s1, char *s2);
void	error(char *str);

//tokenization.c
t_lexer_list	*is_tokenized(char *str);
char 			*n_state(enum e_state state);
char 			*n_type (enum e_token type);
void			add_state(t_lexer_list *head);
void			check_syntax(t_lexer_list *node);
enum e_token	add_type(t_lexer_list *node);

#endif