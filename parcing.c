/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/11 12:36:58 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, enum e_token type)
{
	char *s;
	char *tmp;

	s = 0;
	if(lxr->prev->type == WORD)
		s = str_dup(lxr->prev->content,lxr->prev->len);
	while (lxr && (lxr->type != ' ' || lxr->state != GENERAL) && (lxr->type != '|' || lxr->state != GENERAL))
	{
		if(lxr->type != type || lxr->state != GENERAL)
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


t_excution *parce(t_lexer_list *lxr)
{
	t_excution *execution;
	t_lexer_list *lexer;
	t_list *some;
	enum e_token type;

	if(!lxr)
		return (0);
	some = 0;
	execution = malloc(sizeof(t_excution));
	if(!execution)
		return (0);
	execution->action = malloc(sizeof(t_action));
	if(!execution->action)
		return (0);
	lexer = lxr;
	while (lexer && lexer->type == PIPE_LINE && lexer->state == GENERAL)
	{
		if((lexer->type == DOUBLE_QUOTE && lexer->next->state == IN_DQUOTE ) || (lexer->type == QOUTE && lexer->next->state == IN_QUOTE))
			lexer = fqouts(&some,lexer,lexer->type);
		else if(lexer->type == WORD && lexer->next->type != '\'' && lexer->next->type != '\"')
			add_back_lst(&some,lst_new(str_dup(lexer->content,lexer->len)));	
		else if(lexer->type == REDIR_IN || lexer->type == HERE_DOC)
		{
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			fadd_back_lst(&execution->action->input,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->input->type = type;
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			fadd_back_lst(&execution->action->output,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->output->type = type;
		}
		lexer = lexer->next;
	}
	execution->cmd = getarray(some);
	if(lexer && lexer->type == PIPE_LINE)
	{
		execution->action->pipe = 1;
		lexer = lexer->next;
	}
	else
		execution->action->pipe = 0;
	execution->next = parce(lexer);
	return (execution);
}
