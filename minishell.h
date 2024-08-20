/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:34:27 by haouky            #+#    #+#             */
/*   Updated: 2024/08/20 12:02:43 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

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
}t_lexer_list;

typedef struct  s_out_in_put
{
	enum e_token type;
	char 					*name;
	char 					*s;
	struct  s_out_in_put	*next;
	struct  s_out_in_put	*herdoc_next;
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
t_lexer_list		*new_node(char *str);
void				add_back(t_lexer_list **head, t_lexer_list *new);

//linked_list_utils
void				add_back_lst(t_list **head, t_list *new);
void				fadd_back_lst(t_oip **head, t_oip *new);
int					ft_lstsize(t_list *lst);
t_oip				*flst_new(char *s);
t_list				*lst_new(char *s);
int					cmd_lst_size(t_excution *lst);

//parcing.c
t_excution *parse(t_lexer_list *lxr, t_list *env);

//parcing utils
t_lexer_list		*fqouts(t_list **head,t_lexer_list *lxr, t_list *env);
t_lexer_list		*ftqouts(t_oip **head,t_lexer_list *lxr, enum e_token type, t_list *env);
char				*envv(char *lxr, t_list *env);
char				*get_path(char *s, t_list *env);

//helper func
char				*str_join(char *s1, char *s2);
char				**getarray(t_list *lst);
char				*str_dup(char *str, int size);
char				*ft_substr(char *s, int st, size_t l);
char				**ft_split(char *s, char c);


//tools.c
char				*str_ncopy(char *str, int size);
int					check_char(char c);
int					str_ncomp(char *s1, char *s2, int size);
void				error(char *str);
int					str_len(char *str, char separator);

//tokenization.c
t_lexer_list		*is_tokenized(char *str);
char 				*n_state(enum e_state state);
char 				*n_type (enum e_token type);
void				add_state(t_lexer_list *head);
void				check_syntax(t_lexer_list *node);
enum e_token		add_type(t_lexer_list *node);

//builtins_func.c
void				ft_echo(char **str, char flag);
void				ft_pwd();
void				ft_env(t_list *env);
void				ft_export(t_list **env, char *to_export);
void				ft_unset(t_list **env, char *to_unset);
int					is_exist(t_list *head, t_list **node, char *to_export);
int					check_param(char *str);
t_list				*get_env(char **env);

//excu.c
int					open_in_files(t_oip *list);
int					open_out_files(t_oip *out_file);
void				run_cmd(t_excution *list, char **env);

#endif