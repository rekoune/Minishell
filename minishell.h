/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 11:34:27 by haouky            #+#    #+#             */
/*   Updated: 2024/09/01 09:45:32 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "GET_NEXT_LINE/get_next_line.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <sys/stat.h>

// sturct and

extern int g_status;
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

// ft_error.c
int						ft_error(char *str1, char *str2, char *str3,
							int exit_status);
void					execve_error(char *str, int macro);

// ft_export.c
int						is_empty(char *str);
int						skip_n(char *str);
int						ft_export(t_list **env, char *to_export);
int						is_exist(t_list *head, t_list **node, char *to_export);
t_list					*get_env(char **env);

// ft_unset
int						ft_unset(t_list **env, char *to_unset);
int						check_param(char *str);
t_list					*ft_find(t_list *head, t_list **prev, char *to_unset);

// check_syntax.c
int						check_syntax(t_lexer_list *node);
int						check_redir_synx(t_lexer_list **head);
int						is_redir(enum e_token type);
int						more_check(t_lexer_list *head);
int						check_pipe(t_lexer_list *head);

// execution.c
int						run_execution(t_execution *execution, t_list **env,
							int status);
int						in_fd(t_oip *input, int fd);
int						out_fd(t_oip *output, int fd, int pipe);
void					exccmd(t_execution *exec, t_list *env, int *fd,
							int old_read);

// builtins_func.c
int						ft_env(t_list *env, int fd, int flag);
int						ft_cd(char *str, t_list *env);
int						ft_exit(char **cmd, int exit_status);
int						ft_echo(char **str, int fd);
int						ft_pwd(int fd);

// builtins_utils.c
int						check_builtins(char *str);
int						ft_export_arr(t_list **env, char **cmd, int out_fd);
int						ft_unset_arr(t_list **env, char **arg);
int						execute_builtins(char **cmd, t_list **env, int flag,
							int out_fd);
int						skip_n(char *str);

// free_resources.c
void					free_resources(t_execution *execution);
void					free_lexer(t_lexer_list *list);
void					fr_double(char **s);

// ft_split.c
// static int			cw(char const *s, char c);
// static int			lenw(char const *s, char c);
// void					fr(char **s, int i);
// static char			**msp(char **str, char *s, int word, char c);
char					**ft_split(char *s, char c);

// herdoc.c
t_oip					*get_here_doc(t_execution *execution);
void herdoc_write(int fd, char *s, t_list *env, t_stat *status);
void thedoc(t_oip *herdoc, int fd, t_list *env, t_stat *status);
int						run_here_doc(t_oip *herdoc, t_list *env, int status);

// itoa.c
// static char				*rev(char *c);
char					*ft_itoa(int nb);
void					fr_double(char **s);
int						find_c(char *s, char c);
long					ft_atoi(char *str);

// linked_list.c
void					add_back_lst(t_list **head, t_list *new);
void					fadd_back_lst(t_oip **head, t_oip *new);
t_oip					*flst_new(char *s);
t_list					*lst_new(char *s);
int						cmd_lst_size(t_execution *lst);

// list_utils.c
int						ft_lstsize(t_list *lst);
void					add_back(t_lexer_list **head, t_lexer_list *new);
t_lexer_list			*new_node(char *str);
int						ft_write(char *str, int fd, int flag);

// minishell.c
void					printexec(t_execution *exec);
void					handler(int n);

// parcing.c
t_execution				*parse(t_lexer_list *lxr, t_list *env, int status);

// parcing utils
t_lexer_list			*cmd_arg(t_list **head, t_lexer_list *lxr, t_list *env,
							int status);
t_lexer_list			*redirections(t_oip **head, t_lexer_list *lxr, t_stat *stat,
							t_list *env);
char					*get_varibl(char *variabl, t_list *env, int status);
char					*get_path(char *s, t_list *env);
t_lexer_list			*empty_arg(t_list **head, t_lexer_list *lxr);

// tokenization.c
t_lexer_list			*is_tokenized(char *str);
int						get_spec_char(t_lexer_list **list, char *str, int i,
							int j);
t_lexer_list			*set_quote_state(t_lexer_list *head, enum e_token type);
void					add_state(t_lexer_list *head);
enum e_token			add_type(t_lexer_list *node);
char					*n_state(enum e_state state);
char					*n_type(enum e_token type);

// tools.c
char					*str_ncopy(char *str, int size);
int						check_char(char c);
int						str_ncomp(char *s1, char *s2, int size);
int						error(char *str);
int						str_len(char *str, char separator);

// utils.c
char					**getarray(t_list *lst);
int						find_c(char *s, char c);
char					*str_join(char *s1, char *s2);
char					*str_dup(char *str, int size);
// static char				*sub(char const *s, unsigned int st, size_t l);
char					*ft_substr(char *s, int st, size_t l);

#endif