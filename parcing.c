/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/13 19:53:40 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr)
{
	char *s;
	char *tmp;

	s = 0;
	while (lxr && (lxr->type != ' ' || lxr->state != GENERAL) && (lxr->type != '|' || lxr->state != GENERAL))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
			s = str_join(s,lxr->content);
			free(tmp);
		}
		lxr = lxr->next;
	}
	add_back_lst(head, lst_new(s));
	return (lxr);
}

t_lexer_list  *ftqouts(t_oip **head,t_lexer_list *lxr, enum e_token type)
{
	char *s;
	char *tmp;
	t_oip *node;
	
	s = 0;
	while (lxr && (lxr->type != ' ' || lxr->state != GENERAL) && (lxr->type != '|' || lxr->state != GENERAL))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
			s = str_join(s,lxr->content);
			free(tmp);
		}
		lxr = lxr->next;
	}
	node = flst_new(s);
	node->type = type;
	fadd_back_lst(head, node);
	return (lxr);
}

t_excution *parse(t_lexer_list *lexer)
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
			lexer = ftqouts(&execution->input,lexer, type);
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			lexer = ftqouts(&execution->output,lexer, type);
		}
		else if(lexer->type != WHITE_SPACE)
			lexer = fqouts(&some,lexer);
		if(lexer)
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
	execution->next = parse(lexer);
	return (execution);
}
