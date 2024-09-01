/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:17:28 by arekoune          #+#    #+#             */
/*   Updated: 2024/09/01 09:44:05 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtins(char *str)
{
	if (str && !str_ncomp(str, "echo", 5))
		return (1);
	else if (str && !str_ncomp(str, "cd", 3))
		return (2);
	else if (str && !str_ncomp(str, "pwd", 4))
		return (3);
	else if (str && !str_ncomp(str, "export", 7))
		return (4);
	else if (str && !str_ncomp(str, "unset", 6))
		return (5);
	else if (str && !str_ncomp(str, "env", 4))
		return (6);
	else if (str && !str_ncomp(str, "exit", 5))
		return (7);
	return (0);
}

int	ft_export_arr(t_list **env, char **cmd, int out_fd)
{
	int	i;
	int	exit_status;

	exit_status = 0;
	i = 0;
	if (!cmd[0])
		return (ft_env(*env, out_fd, 1));
	while (cmd[i])
	{
		if (ft_export(env, cmd[i]))
			exit_status = EXIT_FAILURE;
		i++;
	}
	return (exit_status);
}

int	ft_unset_arr(t_list **env, char **arg)
{
	int	i;
	int	exit_status;

	i = 0;
	exit_status = 0;
	while (arg[i])
	{
		if (ft_unset(env, arg[i]))
			exit_status = EXIT_FAILURE;
		i++;
	}
	return (exit_status);
}

int	execute_builtins(char **cmd, t_list **env, int flag, int *out_exit)
{
	if (flag == 1)
		return (ft_echo(cmd, out_exit[0]));
	else if (flag == 2)
		return (ft_cd(cmd[0], *env));
	else if (flag == 3)
		return (ft_pwd(out_exit[0]));
	else if (flag == 4)
		return (ft_export_arr(env, cmd, out_exit[0]));
	else if (flag == 5)
		return (ft_unset_arr(env, cmd));
	else if (flag == 6)
		return (ft_env(*env, out_exit[0], 0));
	else if (flag == 7)
		return (ft_exit(cmd, out_exit[1]));
	return (0);
}

int	skip_n(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
	{
		i++;
		while (str[i] && str[i] == 'n')
			i++;
	}
	if (i == str_len(str, '\0'))
		return (0);
	return (1);
}
