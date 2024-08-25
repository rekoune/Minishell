/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/08/25 16:11:49 by haouky           ###   ########.fr       */
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
	// rl_replace_line("\033[32mminishell \033[0m> ", 1);
}
int main(int ac, char **av, char **env)
{
	char		*str;
	t_lexer_list	*cmd;
	t_execution *execution;
	// int	 i;
	// char *type;
	t_list *enva;
	(void) ac;
	(void) av;
	// int pid = 0;

	signal(SIGINT,handler);
	enva = get_env(env);
	int exit_status = 0;
	while (1)
	{
	
		str = readline("\033[32mminishell \033[0m> ");
		if(!str)
			exit(1);
		if (str[0])
		{
		add_history(str);
		cmd = is_tokenized(str);
		// print(cmd);
		execution = parse(cmd,enva, exit_status);
		free_lexer(cmd);
		// exit_status = run_cmd(execution, &enva);
		exit_status = run_execution(execution, enva);
		free_resources(execution);
		
		// printf("exit status = %d\n", exit_status);
		}
		free(str);
		// open_in_files(execution);
		// if(!execution->input)
	
		// printf("---------------------------------------------------------------------\n");
		// while (execution)
		// {
		// 	printf("******************************************************************\n");
		// 	i = 0;
		// 	while(execution->cmd[i])
		// 		printf("exection>>>>>>>>>>>> == %s\n", execution->cmd[i++]);
		// 	printf("execution->path >>>>>>>>> ==%s\n ",execution->path);
		// 	while (execution->input)
		// 	{
		// 		// printf("/*/checkin \n");
		// 		// if(execution->input->type == REDIR_IN|| execution->input->type == HERE_DOC)
		// 		// 	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>in\n");
		// 		// printf("/*/endcheckin \n");
		// 		type = n_type(execution->input->type);
		// 		printf("input type : %s, name == %s\n", type, execution->input->name);
		// 		execution->input = execution->input->next;
		// 	}
		// 	while (execution->output)
		// 	{
		// 		// printf("/*/checkout \n");
		// 		// if(execution->output->type == REDIR_OUT || execution->output->type == DREDIR_OUT)
		// 		// 	printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>out\n");
		// 		// printf("/*/endcheckout \n");
		// 		type = n_type(execution->output->type);
		// 		printf("input type : %s, name == %s\n", type, execution->output->name);
		// 		execution->output = execution->output->next;
		// 	}
		// 	printf("pipe == %d\n", execution->pipe);
			
		// 	execution = execution->next;
		// }
	}

}
