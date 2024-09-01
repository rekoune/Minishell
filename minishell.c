/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 16:19:43 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_stat1 = 0;

void	print(t_lexer_list *head)
{
	char	*type;
	char	*state;

	printf(" content       len          type                state\n");
	printf("=========================================================\n");
	while (head)
	{
		state = n_state(head->state);
		type = n_type(head->type);
		printf("|%s|         %d,           %s,             %s\n", head->content,
				head->len, type, state);
		head = head->next;
	}
}

void	printexec(t_execution *exec)
{
	t_oip	*head;
	int		i;

	while (exec)
	{
		i = 0;
		printf("path= %s\n", exec->path);
		while (exec->cmd[i])
			printf("cmd = %s\n", exec->cmd[i++]);
		head = exec->input;
		while (head)
		{
			printf("in  | type = %s, name %s,\n", n_type(head->type),
					head->name);
			head = head->next;
		}
		head = exec->output;
		while (head)
		{
			printf("out |type = %s, name %s,\n", n_type(head->type),
					head->name);
			head = head->next;
		}
		exec = exec->next;
	}
}
void	handler(int n)
{
	if (n == SIGINT)
	{
		g_stat1 = 1;
		ft_write("\n", 1, 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}
int	main(int ac, char **av, char **env)
{
	char			*str;
	t_lexer_list	*cmd;
	t_execution		*execution;
	t_list			*enva;
	int				exit_status;

	(void)av;
	// atexit(leaks);
	enva = get_env(env);
	exit_status = 0;
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("minishell$ ");
		if (!str)
			exit(1);
		if (g_stat1)
		{
			exit_status = g_stat1;
			g_stat1 = 0;
		}
		if (str[0])
		{
			add_history(str);
			cmd = is_tokenized(str);
			// print(cmd);
			ac = check_syntax(cmd);
			if (!ac)
			{
				execution = parse(cmd, enva, exit_status);
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
