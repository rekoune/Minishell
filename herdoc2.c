/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:36:57 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 19:31:08 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_oip	*get_here_doc(t_execution *execution)
{
	t_oip	*inp;
	t_oip	*sinp;
	t_oip	*firstehrdoc;

	sinp = 0;
	firstehrdoc = 0;
	while (execution)
	{
		inp = execution->input;
		while (inp)
		{
			if (inp->type == HERE_DOC)
			{
				if (sinp)
					sinp->herdoc_next = inp;
				else
					firstehrdoc = inp;
				sinp = inp;
			}
			inp = inp->next;
		}
		execution = execution->next;
	}
	return (firstehrdoc);
}

void	thedoc(t_oip *herdoc, int fd, t_list *env, t_stat *status)
{
	char	*dlm;
	char	*s;

	dlm = str_join(herdoc->name, "\n");
	write(1, ">", 1);
	s = get_next_line(0);
	while (s)
	{
		if (!str_ncomp(s, dlm, strr_len(dlm)))
			break ;
		if (!herdoc->next)
			herdoc_write(fd, s, env, status);
		free(s);
		write(1, ">", 1);
		s = get_next_line(0);
	}
	free(dlm);
	free(s);
	if (!herdoc->next)
		close(fd);
	exit(0);
}

int	herdoc_w_env(char *s, int fd, int exstat, t_list *env)
{
	int		j;
	char	*tmp;
	char	*tmp2;
	int		i;

	j = 1;
	i = 0;
	while (s[i + j] && alphan(s[i + j]))
		j++;
	if (s[i + 1] == '?')
	{
		tmp2 = ft_itoa(exstat);
		i++;
	}
	else
	{
		tmp = ft_substr(&s[i], 0, j);
		tmp2 = get_varibl(tmp, env, 0);
		free(tmp);
	}
	write(fd, tmp2, strr_len(tmp2));
	free(tmp2);
	return (j);
}

void	herdoc_write(int fd, char *s, t_list *env, t_stat *status)
{
	int		i;
	int		j;

	i = 0;
	while (s[i])
	{
		if (s[i] == '$' && status->type == HERE_DOC)
		{
			j = herdoc_w_env(&s[i], fd, status->exstat, env);
			i += j - 1;
		}
		else
			write(fd, &s[i], 1);
		i++;
	}
}
