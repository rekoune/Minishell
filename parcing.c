/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/12 13:07:54 by arekoune         ###   ########.fr       */
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
	{
		printf("lxr == null\n");
		return (0);
	}
	some = 0;
	execution = malloc(sizeof(t_excution));
	if(!execution)
		return (0);
	execution->action = malloc(sizeof(t_action));
	if(!execution->action)
		return (0);
	lexer = lxr;
	
	printf(">>>>>>>>>>>>>>>>>> %s\n",lexer->content);
	while (lexer && (lexer->type != PIPE_LINE || lexer->state != GENERAL))
	{
		printf("in the while \n");
		if((lexer->type == DOUBLE_QUOTE && lexer->next->state == IN_DQUOTE ) || (lexer->type == QOUTE && lexer->next->state == IN_QUOTE))
		{
			printf("get qouts\n");
			lexer = fqouts(&some,lexer,lexer->type);
			printf("end get qouts\n");
		}
		else if(lexer->type == WORD)
		{
			printf("get word\n");
			if(!lxr->next || (lexer->next->type != '\'' && lexer->next->type != '\"'))
			{
				printf("get word2\n");
				add_back_lst(&some,lst_new(str_dup(lexer->content,lexer->len)));
				printf("get word3\n");
			}
		}
		else if(lexer->type == REDIR_IN || lexer->type == HERE_DOC)
		{
			printf("get input\n");
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			fadd_back_lst(&execution->action->input,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->input->type = type;
		}
		else if(lexer->type == REDIR_OUT || lexer->type == DREDIR_OUT)
		{
			printf("get output\n");
			type = lexer->type;
			lexer = lexer->next;
			if(lexer->type == WHITE_SPACE)
				lexer = lexer->next;
			fadd_back_lst(&execution->action->output,flst_new(str_dup(lexer->content,lexer->len)));
			execution->action->output->type = type;
		}
		lexer = lexer->next;
	}
	printf("end node\n");
	if(lexer)
		printf("22>>>>>>>>>>>>>>>>>> %s\n",lexer->content);
	execution->cmd = getarray(some);
	if(lexer && lexer->type == PIPE_LINE)
	{
		printf("set pipe\n");
		execution->action->pipe = 1;
		lexer = lexer->next;
	}
	else
	{
		printf("set pipe whith zero\n");
		execution->action->pipe = 0;
	}
	sleep(30);
	execution->next = parce(lexer);
	return (execution);
}
