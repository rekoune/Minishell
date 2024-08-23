/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:27:32 by haouky            #+#    #+#             */
/*   Updated: 2024/08/17 10:45:11 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *envv(char *lxr, t_list *env)
{
	int j;
    char *s;
	
	while (env)
	{
		j = 0;
		while (env->str[j] && lxr[j + 1] && lxr[j + 1] == env->str[j])
			j++;
		if(!lxr[j + 1] && env->str[j] == '=')
		{
	        s = str_dup(&env->str[j + 1], str_len(&env->str[j + 1], 0));
            return (s);
		}
		env = env->next;
    }
    return (NULL);
}

char *get_path(char *s, t_list *env)
{
	char *path;
	char *p;
	char **paths;
	int i;

	i = 0;
	
	if(!s)
		return (0);
	if (access(s, X_OK) == 0)
			return (s);
	p = envv("$PATH",env);
	paths = ft_split(p, ':');
	free (p);
	while (paths && paths[i])
	{
		p = str_join(paths[i], "/");
		path = str_join(p, s);
		free(p);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}
t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, t_list *env)
{
	char *s;
	char *tmp;
	char *tmp1;

	s = NULL;
	while (lxr && ((lxr->state != GENERAL) || (lxr->type != ' ' && lxr->type != '|' && lxr->type != '<' && lxr->type != '>' && lxr->type != HERE_DOC && lxr->type != DREDIR_OUT)))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
            if(lxr->type == ENV && lxr->state != IN_QUOTE && lxr->len != 1)
			{
				tmp1 = envv(lxr->content, env);
                s = str_join(s, tmp1);
				free(tmp1);
			}
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
	char *tmp1;
	t_oip *node;
	
	s = NULL;
	while (lxr && ((lxr->state != GENERAL) || (lxr->type != ' ' && lxr->type != '|' && lxr->type != '<' && lxr->type != '>' && lxr->type != HERE_DOC && lxr->type != DREDIR_OUT)))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
			if(lxr->type == ENV && lxr->state != IN_QUOTE && type != HERE_DOC && lxr->len != 1)
			{
				tmp1 = envv(lxr->content, env);
                s = str_join(s, tmp1);
				free(tmp1);
			}
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