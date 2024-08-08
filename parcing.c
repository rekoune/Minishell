/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/08 13:17:04 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *str_join(char *s1, char *s2)
{
	char *s;
	int i;
	int l;

	i = 0;
	l = 0;
	s = malloc(str_len(s1, '\0') + str_len(s1, '\0') + 1);
	while (s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	while (s2[l])
	{
		s[i + l] = s2[l];
		l++;
	}
	s[i + l] = 0;
	return (s);
}

t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, enum e_state state)
{
	char *s;
	char *tmp;
	s = str_dup(lxr->content,lxr->len);
	lxr = lxr->next;
	while (lxr && lxr->state == state)
	{
		tmp = s;
		s = str_join(s,lxr->content);
		free(tmp);
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
	while (lexer && lexer->type == PIPE_LINE)
	{
		if((lexer->type == DOUBLE_QUOTE && lexer->next->state == IN_DQUOTE ) || (lexer->type == QOUTE && lexer->next->state == IN_QUOTE) )
			lexer = fqouts(&some,lexer,lexer->next->state);
		else if(lexer->type == WORD)
			add_back_lst(&some,lst_new(str_dup(lexer->content,lexer->len)));
		else if(lexer->type == REDIR_IN || lexer->type == HERE_DOC)
		{
			lexer = lexer->next;
			fadd_back_lst(&execution->action->input,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->input->type = lexer->prev->type;
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			lexer = lexer->next;
			fadd_back_lst(&execution->action->output,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->output->type = lexer->prev->type;
		}
		lexer = lexer->next;
	}
	execution->cmd = getarray(some);
	if(lexer && lexer->type == PIPE_LINE)
		execution->action->pipe = 1;
	else
		execution->action->pipe = 0;
	execution->next = parce(lexer);
	return (execution);
}
