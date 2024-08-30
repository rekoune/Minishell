/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/08/30 17:19:05 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int stat1 = 0;

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

void printexec(t_execution *exec)
{
	t_oip *head;

	
	while (exec)
	{
		int i = 0;
		printf("path= %s\n",exec->path);
		while (exec->cmd[i])
			printf("cmd = %s\n",exec->cmd[i++]);
		head = exec->input;
		while (head)
		{
			printf("in  | type = %s, name %s,\n",n_type(head->type), head->name);
			head = head->next;
		}
		head = exec->output;
		while (head)
		{
			printf("out |type = %s, name %s,\n",n_type(head->type), head->name);
			head = head->next;
		}
		exec = exec->next;
	}
}
void handler(int n)
{
	n = 1;
	stat1 = n;
	ft_write("\n", 1, 0);
	rl_on_new_line(); 
    rl_replace_line("", 0); 
    rl_redisplay(); 
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
	
		str = readline("minishell$ ");
		if(!str)
			exit(1);
		if(stat1 )
		{
			if(!exit_status)
				exit_status = stat1;
			stat1 = 0;
		}
		if (str[0])
		{
			add_history(str);
			cmd = is_tokenized(str);
			// print(cmd);
			ac = check_syntax(cmd);
			if(!ac)
			{
				execution = parse(cmd,enva, exit_status);
				// printexec(execution);
				free_lexer(cmd);
				exit_status = run_execution(execution, &enva, exit_status);
				free_resources(execution);
			}
			else
			{
				free_lexer(cmd);
				exit_status = ac;
			}
		}
		free(str);
	}

}
