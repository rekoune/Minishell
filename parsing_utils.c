/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:27:32 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 19:14:50 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*cmd_arg2(t_lexer_list *lxr, t_list *env, char *s, int status)
{
	char	*tmp1;

	if (lxr->type == '$' && lxr->state == IN_DQUOTE && lxr->len != 1)
	{
		tmp1 = get_varibl(lxr->content, env, status);
		s = str_join(s, tmp1);
		free(tmp1);
	}
	else
		s = str_join(s, lxr->content);
	return (s);
}

t_lexer_list	*cmd_arg(t_list **head, t_lexer_list *lxr, t_list *env,
		int status)
{
	char			*s;
	char			*tmp;
	enum e_token	type;

	s = NULL;
	while (lxr && redpip(lxr))
	{
		if ((lxr->type != '\'' && lxr->type != '\"') || lxr->state != GENERAL)
		{
			tmp = s;
			if (lxr->type == ENV && lxr->state == GENERAL && lxr->len != 1)
				s = env4_arg(get_varibl(lxr->content, env, status), s, head);
			else
				s = cmd_arg2(lxr, env, s, status);
			free(tmp);
		}
		type = lxr->type;
		lxr = lxr->next;
	}
	if (!s && type != ENV)
		s = str_dup("", 0);
	if (s)
		add_back_lst(head, lst_new(s));
	return (lxr);
}

char	*reder(t_lexer_list *lxr, t_stat *stat, t_list *env, char *s)
{
	char	*tmp1;

	if (lxr->type == ENV && lxr->state != IN_QUOTE && stat->type != HERE_DOC
		&& lxr->len != 1)
	{
		tmp1 = get_varibl(lxr->content, env, stat->exstat);
		if (lxr->state == GENERAL)
		{
			tmp1 = env_for_red(tmp1, s, &stat->exstat, lxr->next);
			if (stat->exstat == -1000)
				stat->type = WORD;
			if (stat->exstat == -1000)
				s = str_join(s, lxr->content);
			else
				s = str_join(s, tmp1);
			free(tmp1);
		}
		if (lxr->state != GENERAL)
			s = str_join(s, tmp1);
		if (lxr->state != GENERAL)
			free(tmp1);
	}
	else
		s = str_join(s, lxr->content);
	return (s);
}

void	setupinout(t_oip **head, enum e_state st, t_stat *stat, char *s)
{
	t_oip	*node;

	node = flst_new(s);
	node->type = stat->type;
	if (stat->type == HERE_DOC && st != GENERAL)
		node->s = str_dup("", 0);
	else
		node->s = NULL;
	fadd_back_lst(head, node);
}

t_lexer_list	*redirections(t_oip **head, t_lexer_list *lxr, t_stat *stat,
		t_list *env)
{
	char			*s;
	char			*tmp;
	enum e_state	st;

	s = NULL;
	st = GENERAL;
	while (lxr && redpip(lxr))
	{
		if ((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE)
			|| lxr->state != GENERAL)
		{
			tmp = s;
			s = reder(lxr, stat, env, s);
			free(tmp);
			if (lxr->state != GENERAL)
				st = lxr->state;
		}
		if (!s)
			s = str_dup("", 0);
		lxr = lxr->next;
	}
	setupinout(head, st, stat, s);
	return (lxr);
}
