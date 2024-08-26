/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:34:27 by haouky            #+#    #+#             */
/*   Updated: 2024/08/26 19:21:55 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "GET_NEXT_LINE/get_next_line.h"
# include "ft_printf/ft_printf.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
// # include "/Users/haouky/.brew/opt/readline/include/readline/readline.h"
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

//sturct and
typedef struct s_list
{
	char				*str;
	struct s_list		*next;
}						t_list;

enum					e_token
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

enum					e_state
{
	IN_DQUOTE,
	IN_QUOTE,
	GENERAL,
};

typedef struct s_stat
{
	enum e_token		type;
	int					exstat;
}						t_stat;
typedef struct s_lexer_list
{
	char				*content;
	int					len;
	enum e_token		type;
	enum e_state		state;
	int					exstatus;
	struct s_lexer_list	*next;
}						t_lexer_list;

typedef struct s_out_in_put
{
	enum e_token		type;
	char				*name;
	char				*s;
	struct s_out_in_put	*next;
	struct s_out_in_put	*herdoc_next;
}						t_oip;

typedef struct s_execution
{
	char				**cmd;
	char				*path;
	t_oip				*input;
	t_oip				*output;
	int					pipe;
	struct s_execution	*next;
}						t_execution;

//minishell.c

//list_utils.c
t_lexer_list			*new_node(char *str);
void					add_back(t_lexer_list **head, t_lexer_list *new);

//linked_list_utils
void					add_back_lst(t_list **head, t_list *new);
void					fadd_back_lst(t_oip **head, t_oip *new);
int						ft_lstsize(t_list *lst);
t_oip					*flst_new(char *s);
t_list					*lst_new(char *s);
int						cmd_lst_size(t_execution *lst);

//parcing.c
t_execution				*parse(t_lexer_list *lxr, t_list *env, int status);

//parcing utils
t_lexer_list			*fqouts(t_list **head, t_lexer_list *lxr, t_list *env,
							int status);
t_lexer_list			*ftqouts(t_oip **head, t_lexer_list *lxr, t_stat *stat,
							t_list *env);
char					*envv(char *lxr, t_list *env, int status);
char					*get_path(char *s, t_list *env);

//helper func
char					*str_join(char *s1, char *s2);
char					**getarray(t_list *lst);
char					*str_dup(char *str, int size);
int						str_len(char *str, char separator);
int						ft_strncmp(char *s1, char *s2, int n);
char					*ft_substr(char *s, int st, size_t l);
char					**ft_split(char *s, char c);
char					*get_next_line(int fd);

//tools.c
char					*str_ncopy(char *str, int size);
int						check_char(char c);
int						str_ncomp(char *s1, char *s2, int size);
int						error(char *str);
int						str_len(char *str, char separator);
char					*ft_itoa(int nb);

//tokenization.c
t_lexer_list			*is_tokenized(char *str);
char					*n_state(enum e_state state);
char					*n_type(enum e_token type);
void					add_state(t_lexer_list *head);
int						check_syntax(t_lexer_list *node);
enum e_token			add_type(t_lexer_list *node);

//builtins_func.c
int						ft_echo(char **str, int fd);
int						ft_pwd(int fd);
int						ft_env(t_list *env, int fd, int flag);
int						ft_export(t_list **env, char *to_export, int fd);
int						ft_unset(t_list **env, char *to_unset);
int						is_exist(t_list *head, t_list **node, char *to_export);
int						check_param(char *str);
t_list					*get_env(char **env);
int						ft_cd(char *str, t_list *env);
int						ft_exit(char **cmd);
int						ft_write(char *str, int fd);

//excu.c
int						open_in_files(t_oip *list);
int						open_out_files(t_oip *out_file);
int						run_cmd(t_execution *list, t_list **env);

//excu tools
int						check_builtins(char *str);
int						execute_builtins(char **cmd, t_list **env, int flag, int out_fd);

//utils.c
char					**getarray(t_list *lst);
int						find_c(char *s, char c);

//execution
int						run_execution(t_execution *execution, t_list *env, int status);
int 					run_here_doc(t_oip *herdoc, t_list *env, int status);
t_oip					*get_here_doc(t_execution *execution);

//free_resources.c
void					free_resources(t_execution *execution);
void					free_lexer(t_lexer_list *list);
void					fr_double(char **s);


#endif