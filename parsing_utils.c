/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:27:32 by haouky            #+#    #+#             */
/*   Updated: 2024/08/14 13:25:33 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *envv(t_lexer_list *lxr, t_list *env)
{
    int i;
	int j;
	int size;
    char *tmp1;
	char *tmp;
    char *p;
    
	i = 0;
	while (lxr->content[i] != '$')
		i++;
	size = str_len(&lxr->content[i + 1], 0);
	while (env)
	{
		j = 0;
		while (env->str[j] && j < size && lxr->content[i + j + 1] == env->str[j])
			j++;
		if(!lxr->content[i + j + 1] && env->str[j] == '=')
		{
			tmp = str_dup(lxr->content, i);
	        tmp1 = str_dup(&env->str[j + 1], str_len(&env->str[j + 1], 0));	
            p = str_join(tmp, tmp1);
            free(tmp);
	        free(tmp1);
            return (p);
		}
		env = env->next;
    }
    return (0);
}
t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, t_list *env)
{
	char *s;
	char *tmp;

	s = 0;
	while (lxr && (lxr->type != ' ' || lxr->state != GENERAL) && (lxr->type != '|' || lxr->state != GENERAL))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
            if(lxr->type == ENV && lxr->state != IN_QUOTE)
                s = str_join(s, envv(lxr, env));
            else 
			    s = str_join(s,lxr->content);
			free(tmp);
		}
		lxr = lxr->next;
	}
	add_back_lst(head, lst_new(s));
	return (lxr);
}

t_lexer_list  *ftqouts(t_oip **head,t_lexer_list *lxr, enum e_token type, t_list  *env)
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
			if(lxr->type == ENV && lxr->state != IN_QUOTE)
                s = str_join(s, envv(lxr, env));
            else 
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
