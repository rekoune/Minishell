#ifndef MINISHELL_H
#define MINISHELL_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_list
{
	char *str;
	void *next;
}t_list;


//minishell.c

//list_utils.c

//ft_split.c
char		**ft_split(char *str, char separator);
int 		word_counter(char *str, char	separator);
char		*str_dup(char *str, int size);
int			str_len(char *str, char separator);

//parcing.c
char 		**parcing(char *str);

//sturct and 
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
	enum e_token state_before;
	char *filebefore;
	enum e_token state_after;
	char *fileafter;
	int pipe;
}   t_action;
typedef struct s_excution
{
	char **cmd;
	char *path;
	t_action action;
} t_excution;

#endif