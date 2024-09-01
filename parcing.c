/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 17:03:55 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *s, t_list *env)
{
	char	*path;
	char	*p;
	char	**paths;
	int		i;

	i = 0;
	if (!s)
		return (0);
	if (access(s, X_OK) == 0 || find_c(s, '/'))
		return (str_dup(s, str_len(s, '\0')));
	p = get_varibl("$PATH", env, 0);
	paths = ft_split(p, ':');
	free(p);
	while (paths && paths[i])
	{
		p = str_join(paths[i], "/");
		path = str_join(p, s);
		free(p);
		if (access(path, X_OK) == 0)
			return (fr_double(paths), path);
		free(path);
		i++;
	}
	fr_double(paths);
	return (NULL);
}

char	**parse2(t_lexer_list **lexer, t_execution *execution, t_list **ev,
		t_stat stat)
{
	while (*lexer && ((*lexer)->type != '|' || (*lexer)->state != GENERAL))
	{
		if ((*lexer)->type == REDIR_IN || (*lexer)->type == HERE_DOC)
		{
			stat.type = (*lexer)->type;
			(*lexer) = (*lexer)->next;
			while ((*lexer)->type == WHITE_SPACE)
				(*lexer) = (*lexer)->next;
			(*lexer) = redirections(&execution->input, (*lexer), &stat, ev[0]);
		}
		else if ((*lexer)->type == REDIR_OUT || (*lexer)->type == DREDIR_OUT)
		{
			stat.type = (*lexer)->type;
			(*lexer) = (*lexer)->next;
			while ((*lexer)->type == WHITE_SPACE)
				(*lexer) = (*lexer)->next;
			(*lexer) = redirections(&execution->output, (*lexer), &stat, ev[0]);
		}
		else if ((*lexer)->type != WHITE_SPACE)
			(*lexer) = cmd_arg(&ev[1], (*lexer), ev[0], stat.exstat);
		else
			(*lexer) = (*lexer)->next;
	}
	return (getarray(ev[1]));
}

t_execution	*parse(t_lexer_list *lexer, t_list *env, int status)
{
	t_execution	*execution;
	t_stat		stat;
	t_list		*ev[2];

	if (!lexer)
		return (NULL);
	execution = malloc(sizeof(t_execution));
	if (!execution)
		return (NULL);
	execution->input = NULL;
	execution->output = NULL;
	stat.exstat = status;
	ev[0] = env;
	ev[1] = NULL;
	execution->cmd = parse2(&lexer, execution, ev, stat);
	if (lexer && lexer->type == PIPE_LINE)
	{
		execution->pipe = 1;
		lexer = lexer->next;
	}
	else
		execution->pipe = 0;
	execution->path = get_path(execution->cmd[0], env);
	execution->next = parse(lexer, env, status);
	return (execution);
}
