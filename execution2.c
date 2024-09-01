/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 19:50:39 by haouky            #+#    #+#             */
/*   Updated: 2024/09/01 10:26:29 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	in_fd(t_oip *input, int fd)
{
	while (input)
	{
		if (fd)
			close(fd);
		if (input->type == REDIR_IN)
			fd = open(input->name, O_RDONLY);
		if (!input->next && input->type == HERE_DOC)
			fd = open(input->s, O_RDONLY);
		if (input->type == WORD)
			return (ft_error(input->name, ": ambiguous redirect", NULL, -1));
		if (fd < 0)
		{
			if (!input->name || !input->name[0])
				ft_error(": No such file or directory", NULL, NULL, 0);
			else
				ft_error(input->name, ": ", strerror(errno), 0);
			return (-1);
		}
		input = input->next;
	}
	return (fd);
}

int	out_fd(t_oip *output, int fd, int pipe)
{
	if (!output && !pipe)
		return (1);
	while (output)
	{
		if (fd != 1)
			close(fd);
		if (output->type == REDIR_OUT)
			fd = open(output->name, O_CREAT | O_TRUNC | O_WRONLY, 0640);
		else if (output->type == DREDIR_OUT)
			fd = open(output->name, O_CREAT | O_APPEND | O_WRONLY, 0640);
		if (output->type == WORD)
			return (ft_error(output->name, ": ambiguous redirect", NULL, -1));
		if (fd < 0)
		{
			if (!output->name || !output->name[0])
				ft_error(": No such file or directory", NULL, NULL, 0);
			else
				ft_error(output->name, ": ", strerror(errno), 0);
			return (-1);
		}
		output = output->next;
	}
	return (fd);
}

void	setup_files(t_execution *exec, t_list *env, int *fd, int old_read)
{
	int	fd[2];
	int	st[2];

	fd[0] = in_fd(exec->input, old_read);
	if (fd[0] == -1)
		exit(1);
	fd[1] = out_fd(exec->output, fd[1], exec->pipe);
	if (fd[1] == -1)
		exit(1);
	dup2(fd[0], 0);
	if (fd[0])
		close(fd[0]);
	if (fd[0])
		close(fd[0]);
	dup2(fd[1], 1);
	if (fd[1] != 1)
		close(fd[1]);
	if (exec->cmd[0])
		fd[0] = check_builtins(exec->cmd[0]);
	if (exec->cmd && exec->cmd[0] && fd[0])
	{
		st[0] = 1;
		fd[0] = execute_builtins(&exec->cmd[1], &env, fd[0], st);
		exit(fd[0]);
	}
}

void	exccmd(t_execution *exec, t_list *env, int *fd, int old_read)
{
	struct stat	state;

	setup_files(exec, env, fd, old_read);
	if ((exec->cmd[0] && !exec->path) || (exec->cmd[0] && !exec->cmd[0][0]))
	{
		if (!get_varibl("$PATH", env, 0))
			ft_error(exec->cmd[0], ": No such file or directory", NULL, 0);
		else
			ft_error(exec->cmd[0], ": command not found", NULL, 0);
		exit(127);
	}
	if (!exec->cmd[0])
		exit(0);
	if (stat(exec->cmd[0], &state) == 0 && S_ISDIR(state.st_mode))
		exit(ft_error(exec->cmd[0], ": is a directory", NULL, 126));
	if (execve(exec->path, exec->cmd, getarray(env)) == -1)
		execve_error(exec->cmd[0], errno);
}
