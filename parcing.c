/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parcing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:16:25 by haouky            #+#    #+#             */
/*   Updated: 2024/08/06 15:28:45 by haouky           ###   ########.fr       */
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

t_list *lst_new(char *s)
{
	t_list *lst;

	lst = malloc (sizeof(t_list));
	if(!lst)
		return (0);
	lst->str = s;
	lst->next = 0;	
}

void    add_back_lst(t_list **head, t_list *new)
{
    t_lexer_list *node;
    node = *head;
    if(!head)
        return;
    if(*head == NULL)
    {
        (*head) = new;
        return;
    }
    while(node->next)
        node = node->next;
    node->next = new;
}

t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, enum e_state state)
{
	char *s;

	s = str_dup(lxr->content,lxr->len);
	lxr = lxr->next;
	while (lxr && lxr->state == state)
	{
		s = str_join(s,lxr->content);
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
	
	some = 0;
	execution = malloc(sizeof(t_excution));
	lexer = lxr;
	while (lexer && lexer->type == PIPE_LINE)
	{
		if((lexer->type == DOUBLE_QUOTE && lexer->next->state == IN_DQUOTE ) ||(lexer->type == QOUTE && lexer->next->state == IN_QUOTE) )
			lexer = fqouts(&some,lexer,lexer->next->state);
		else if(lexer->type == WORD);
			add_back_lst(&some,lst_new(str_dup(lexer->content,lexer->len)));
		else if(lexer->type == REDIR_IN)
		{
				
		}
		lexer = lexer->next;
	}
}
