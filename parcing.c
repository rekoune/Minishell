/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/17 10:44:01 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_excution *parse(t_lexer_list *lexer, t_list *env)
{
	t_excution *execution;
	t_list *some;
	enum e_token type;

	if(!lexer)
		return (NULL);
	some = NULL;
	execution = malloc(sizeof(t_excution));
	if(!execution)
		return (NULL);
	execution->input = NULL;
	execution->output = NULL;
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
	execution->path = get_path(execution->cmd[0], env);
	execution->next = parse(lexer, env);
	return (execution);
}
