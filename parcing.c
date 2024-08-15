/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/15 09:59:48 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_excution *parse(t_lexer_list *lexer, t_list *env)
{
	t_excution *execution;
	t_list *some;
	enum e_token type;

	if(!lexer)
		return (0);
	some = 0;
	execution = malloc(sizeof(t_excution));
	if(!execution)
		return (0);
	execution->input = 0;
	execution->output = 0;
	while (lexer && (lexer->type != PIPE_LINE || lexer->state != GENERAL))
	{
		if(lexer->type == REDIR_IN || lexer->type == HERE_DOC)
		{
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			lexer = ftqouts(&execution->input,lexer, type, env);
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			lexer = ftqouts(&execution->output,lexer, type, env);
		}
		else if(lexer->type != WHITE_SPACE)
			lexer = fqouts(&some,lexer, env);
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
	execution->next = parse(lexer, env);
	return (execution);
}
