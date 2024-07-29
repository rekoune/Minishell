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

//minishell.c

//list_utils.c
t_cmd_list	*new_node(char **str);
void		add_back(t_cmd_list **head, t_cmd_list *new);

//ft_split.c
char		**ft_split(char *str, char separator);
int 		word_counter(char *str, char	separator);
char		*str_dup(char *str, int size);
int			str_len(char *str, char separator);

//parcing.c
char 		**parcing(char *str);

#endif