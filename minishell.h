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
	char 			*str;
	struct s_list	*next;
}t_list;

enum e_token
{
	WORD = -1,
	WHITE_SPACE = ' ',
	QOUTE = '\'',
	DOUBLE_QUOTE = '\"',
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

typedef struct  s_out_in_put
{
	enum e_token type;
	char *name;
	char *s;
	struct  s_out_in_put *next;
	struct  s_out_in_put *herdoc_next;
}    t_oip;

typedef struct s_excution
{
	char **cmd;
	char *path;
	t_oip *input;
	t_oip *output;
	int pipe;
	struct s_excution *next;
} t_excution;


//minishell.c

//list_utils.c
t_lexer_list	*new_node(char *str);
void			add_back(t_lexer_list **head, t_lexer_list *new);

//linked_list_utils
void			add_back_lst(t_list **head, t_list *new);
void			fadd_back_lst(t_oip **head, t_oip *new);
int				ft_lstsize(t_list *lst);
t_oip			*flst_new(char *s);
t_list			*lst_new(char *s);

//parcing.c
t_excution *parse(t_lexer_list *lxr, t_list *env);

//parcing utils
t_lexer_list 	 *fqouts(t_list **head,t_lexer_list *lxr, t_list *env);
t_lexer_list 	 *ftqouts(t_oip **head,t_lexer_list *lxr, enum e_token type, t_list *env);
char *envv(char *lxr, t_list *env);
char *get_path(char *s, t_list *env);

//helper func
char 	*str_join(char *s1, char *s2);
char	**getarray(t_list *lst);
char		*str_dup(char *str, int size);
int			str_len(char *str, char separator);
int	     ft_strncmp(char *s1, char *s2, int n);
char	*ft_substr(char *s, int st, size_t l);
char	**ft_split(char *s, char c);


//tools.c
char			*str_ncopy(char *str, int size);
int				check_char(char c);
int				str_comp(char *s1, char *s2);
void			error(char *str);
int				str_len(char *str, char separator);

//tokenization.c
t_lexer_list	*is_tokenized(char *str);
char 			*n_state(enum e_state state);
char 			*n_type (enum e_token type);
void			add_state(t_lexer_list *head);
void			check_syntax(t_lexer_list *node);
enum e_token	add_type(t_lexer_list *node);

//builtins_func.c
t_list			*get_env(char **env);

#endif