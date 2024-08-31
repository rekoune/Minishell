/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 12:22:24 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_execution *parse(t_lexer_list *lexer, t_list *env, int status)
{
	t_execution *execution;
	t_list *some;
	t_stat stat;

	if(!lexer)
		return (NULL);
	some = NULL;
	execution = malloc(sizeof(t_execution));
	if(!execution)
		return (NULL);
	execution->input = NULL;
	execution->output = NULL;
	stat.exstat = status;
	while (lexer && (lexer->type != PIPE_LINE || lexer->state != GENERAL))
	{
		if(lexer->type == REDIR_IN || lexer->type == HERE_DOC)
		{
			stat.type = lexer->type;
			lexer = lexer->next;
			while(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			lexer = redirections(&execution->input,lexer, &stat, env);
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			stat.type = lexer->type;
			lexer = lexer->next;
			while(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			lexer = redirections(&execution->output,lexer, &stat, env);
		}
		else if(lexer->type != WHITE_SPACE)
			lexer = cmd_arg(&some,lexer, env, status);
		else
			lexer = lexer->next;
	}
	execution->cmd = getarray(some);
	if(lexer && lexer->type == PIPE_LINE)
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
