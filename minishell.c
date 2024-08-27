/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/08/27 10:52:43 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
    // rl_replace_line("  ", 0); 
	// rl_on_new_line(); 
    // rl_redisplay(); 
	// ft_write("\n\033[32mminishell \033[0m$ ", 1);
}
void leaks()
{
	system("leaks -q minishell");
}
int main(int ac, char **av, char **env)
{
	char		*str;
	t_lexer_list	*cmd;
	t_execution *execution;
	t_list *enva;
	(void) av;

	// atexit(leaks);
	signal(SIGINT, handler);
	signal(SIGQUIT, SIG_IGN);
	enva = get_env(env);
	int exit_status = 0;
	while (1)
	{
	
		str = readline("\033[32mminishell \033[0m$ ");
		if(!str)
			exit(1);
		if (str[0])
		{
			add_history(str);
			cmd = is_tokenized(str);
			ac = check_syntax(cmd);
			if(!ac)
			{
				execution = parse(cmd,enva, exit_status);
				free_lexer(cmd);
				exit_status = run_execution(execution, enva, exit_status);
				free_resources(execution);
			}
			else
			{
				free_lexer(cmd);
				exit_status = ac;
			}
			
			printf("exit status = %d\n", exit_status);
		}
		free(str);
	}

}
