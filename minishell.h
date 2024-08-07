#ifndef MINISHELL_H
#define MINISHELL_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

//sturct and 
typedef struct s_list
{
	char *str;
	void *next;
}t_list;

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

enum e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	GENERAL,
};

typedef struct    s_lexer_list
{
    char                *content;
    int                  len;
    enum e_token        type;
    enum e_state        state;
    struct    s_lexer_list             *next;
    struct    s_lexer_list         *prev;
}t_lexer_list;

typedef struct  s_work_before_cmd
{
	t_list *input;
	t_list *output;
	int pipe;
}   t_action;
typedef struct s_excution
{
	char **cmd;
	char *path;
	t_action *action;
	struct s_excution *next;
} t_excution;

//minishell.c

//list_utils.c
void    add_back_lst(t_list **head, t_list *new);
t_list *lst_new(char *s);

//ft_split.c
char		**ft_split(char *str, char separator);
int 		word_counter(char *str, char	separator);
char		*str_dup(char *str, int size);
int			str_len(char *str, char separator);

//parcing.c
char 		**parcing(char *str);
t_excution *parce(t_lexer_list *lxr);

//parcing utils
t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, enum e_state state);

//helper func
char *str_join(char *s1, char *s2);

#endif