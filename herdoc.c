/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 19:48:20 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 19:47:30 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exit_herdoc(int sig)
{
	(void)sig;
	printf("\n");
	exit(1);
}

int	open_herdoc_file(int i, t_oip *herdoc, t_stat *status)
{
	int		fd;
	char	*tmp;

	tmp = ft_itoa(i);
	if (herdoc->s && !herdoc->s[0])
	{
		status->type = WORD;
		free(herdoc->s);
	}
	else
		status->type = HERE_DOC;
	herdoc->s = str_join("/tmp/", tmp);
	free(tmp);
	fd = open(herdoc->s, O_RDWR | O_CREAT | O_TRUNC, 0640);
	return (fd);
}

int	fork_4_herdoc(t_oip *herdoc, int fd, t_list *env, t_stat status)
{
	int	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("minishel:");
		return (-1);
	}
	if (pid == 0)
	{
		signal(SIGINT, exit_herdoc);
		thedoc(herdoc, fd, env, &status);
	}
	wait(&pid);
	if (!herdoc->next)
		close(fd);
	if (WIFEXITED(pid))
		pid = WEXITSTATUS(pid);
	else if (WIFSIGNALED(pid))
		pid = WTERMSIG(pid) + 128;
	return (pid);
}

int	run_here_doc(t_oip *herdoc, t_list *env, int statu)
{
	int		pid;
	int		fd;
	int		i;
	t_stat	status;

	i = 0;
	pid = 0;
	status.exstat = statu;
	signal(SIGINT, SIG_IGN);
	while (herdoc && !pid)
	{
		if (!herdoc->next)
			fd = open_herdoc_file(i, herdoc, &status);
		pid = fork_4_herdoc(herdoc, fd, env, status);
		i++;
		herdoc = herdoc->herdoc_next;
	}
	return (pid);
}
