/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/08/21 12:12:40 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int g_status = 1;

void print(t_lexer_list *head)
{
	char *type;
	char *state;

	printf(" content       len          type                state\n");
		printf("=========================================================\n");
	while (head)
	{
		state = n_state(head->state);
		type = n_type(head->type);
		
		printf("|%s|         %d,           %s,             %s\n",head->content, head->len, type, state);
		head = head->next;
	}
}
void handler(int n)
{
	n = 0;
}
int main(int ac, char **av, char **env)
{
	char		*str;
	t_lexer_list	*cmd;
	t_excution *exuction;
	// int	 i;
	// char *type;
	t_list *enva;
	(void) ac;
	(void) av;
	int pid = 0;

	// signal(SIGINT,handler);
	enva = get_env(env);
	while (1)
	{
		str = readline("\033[32mminishell \033[0m> ");
		if(str)
		{ 
			add_history(str);
			cmd = is_tokenized(str);
			exuction = parse(cmd,get_env(env));
			printf("---------------------------------------------------------------------\n");
			// signal(SIGINT, SIG_DFL);
			run_exuction(exuction, enva);
			
			printf("endstatus= %d\n",g_status);
		}
	}

}

