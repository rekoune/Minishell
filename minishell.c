/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/15 12:56:50 by haouky            #+#    #+#             */
/*   Updated: 2024/09/01 11:46:03 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_status;

void	handler(int n)
{
	if (n == SIGINT)
	{
		g_status = 1;
		ft_write("\n", 1, 0);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	ft_minishell(char *str, t_list **env, int *exit_status)
{
	t_lexer_list	*cmd;
	t_execution		*execution;
	int				flag;

	add_history(str);
	cmd = is_tokenized(str);
	flag = check_syntax(cmd);
	if (!flag)
	{
		execution = parse(cmd, *env, *exit_status);
		free_lexer(cmd);
		*exit_status = run_execution(execution, env, *exit_status);
		free_resources(execution);
	}
	else
	{
		free_lexer(cmd);
		*exit_status = flag;
	}
}

int	main(int ac, char **av, char **env)
{
	char	*str;
	t_list	*enva;
	int		exit_status;

	(void)av;
	(void)ac;
	enva = get_env(env);
	exit_status = 0;
	while (1)
	{
		signal(SIGINT, handler);
		signal(SIGQUIT, SIG_IGN);
		str = readline("minishell$ ");
		if (!str)
			exit(exit_status);
		if (g_status)
		{
			exit_status = g_status;
			g_status = 0;
		}
		if (str[0])
			ft_minishell(str, &enva, &exit_status);
		free(str);
	}
}
