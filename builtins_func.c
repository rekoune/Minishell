/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 17:17:21 by arekoune          #+#    #+#             */
/*   Updated: 2024/08/31 09:43:17 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_echo(char **str, int fd)
{
	int		i;
	char	flag;

	i = 0;
	flag = 'n';
	if (!str)
		return (EXIT_FAILURE);
	while (str[i] && str[i][0] && (!str_ncomp(str[i], "-n", 3)
			|| !skip_n(str[i])))
	{
		flag = 'y';
		i++;
	}
	while (str[i])
	{
		if (ft_write(str[i], fd, 0) == EXIT_FAILURE)
			return (EXIT_FAILURE);
		if (str[i + 1] != NULL)
			if (ft_write(" ", fd, 0) == EXIT_FAILURE)
				return (EXIT_FAILURE);
		i++;
	}
	if (flag == 'n')
		ft_write("\n", fd, 0);
	return (EXIT_SUCCESS);
}

int	ft_pwd(int fd)
{
	char	buff[1024];

	if (getcwd(buff, sizeof(buff)) == NULL)
	{
		ft_write("minishell: No such file or directory\n", 2, 0);
		return (EXIT_FAILURE);
	}
	if (ft_write(buff, fd, 0) == EXIT_FAILURE || ft_write("\n", fd,
			0) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_env(t_list *env, int fd, int flag)
{
	while (env)
	{
		if (flag == 1)
		{
			ft_write("declare -x ", fd, 0);
			ft_write(env->str, fd, 1);
			ft_write("\n", fd, 0);
		}
		else if (!is_empty(env->str))
		{
			ft_write(env->str, fd, 0);
			ft_write("\n", fd, 0);
		}
		env = env->next;
	}
	return (EXIT_SUCCESS);
}

int	ft_cd(char *str, t_list *env)
{
	char	*s;

	s = NULL;
	if (!str)
	{
		s = envv("$HOME", env, 0);
		if (!s)
			return (ft_error("cd: HOME not set", NULL, NULL, EXIT_FAILURE));
		if (chdir(s) == -1)
		{
			free(s);
			return (ft_error("cd: ", s, ": No such file or directory",
					EXIT_FAILURE));
		}
	}
	else if (str[0] && chdir(str) == -1)
		return (ft_error("cd: ", str, ": No such file or directory",
				EXIT_FAILURE));
	free(s);
	return (EXIT_SUCCESS);
}

int	ft_exit(char **arg)
{
	int	status;

	status = 0;
	printf("exit\n");
	if (!arg[0])
		exit(status);
	status = ft_atoi(arg[0]);
	if (arg[1])
		ft_error("exit: too many arguments", NULL, NULL, 0);
	else
		exit(status);
	return (EXIT_FAILURE);
}
