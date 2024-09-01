/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/22 09:00:40 by haouky            #+#    #+#             */
/*   Updated: 2024/09/01 10:28:14 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sig_handl(int sig)
{
	if (sig == SIGINT)
		write(1, "\n", 1);
	else if (sig == SIGQUIT)
		write(1, "Quit: 3\n", 8);
}

int	exucut(t_execution *execution, int *fd, t_list **env)
{
	int	pid;
	int	oldread;

	oldread = fd[0];
	if (execution->pipe)
	{
		if (pipe(fd) == -1)
		{
			perror("minishel: pipe: ");
			return (-11);
		}
	}
	pid = fork();
	if (pid == 1)
	{
		perror("minishell: fork: ");
		return (1);
	}
	if (pid == 0)
		exccmd(execution, *env, fd, oldread);
	if (oldread)
		close(oldread);
	if (fd[1] != 1)
		close(fd[1]);
	return (pid);
}

int	work(t_execution *execution, t_list **env, int *fd)
{
	int		size;
	int		statu;
	pid_t	*pid;
	int		i;

	size = cmd_lst_size(execution);
	pid = malloc(sizeof(pid_t) * size);
	i = 0;
	while (execution)
	{
		pid[i++] = exucut(execution, fd, env);
		execution = execution->next;
	}
	if (fd[0])
		close(fd[0]);
	i = 0;
	while (i < size)
		waitpid(pid[i++], &statu, 0);
	free(pid);
	if (WIFEXITED(statu))
		statu = WEXITSTATUS(statu);
	else if (WIFSIGNALED(statu))
		statu = WTERMSIG(statu) + 128;
	return (statu);
}

int	builtins(t_execution *execution, t_list **env, int i, int status)
{
	int	fd[2];

	fd[0] = in_fd(execution->input, 0);
	if (fd[0] == -1)
		return (1);
	if (fd[0])
		close(fd[0]);
	fd[0] = out_fd(execution->output, 1, 0);
	if (fd[0] == -1)
		return (1);
	fd[1] = status;
	i = execute_builtins(&execution->cmd[1], env, i, fd);
	if (fd[1] != 1)
		close(fd[1]);
	return (i);
}

int	run_execution(t_execution *execution, t_list **env, int status)
{
	t_oip	*herdoc;
	int		fd[2];
	int		i;
	int		statu;

	i = 0;
	statu = 0;
	herdoc = get_here_doc(execution);
	if (herdoc)
		statu = run_here_doc(herdoc, *env, status);
	if (statu)
		return (statu);
	signal(SIGINT, sig_handl);
	signal(SIGQUIT, sig_handl);
	fd[0] = 0;
	fd[1] = 1;
	if (execution->cmd[0])
		i = check_builtins(execution->cmd[0]);
	if (i && !execution->pipe)
		return (builtins(execution, env, i, status));
	return (work(execution, env, fd));
}
