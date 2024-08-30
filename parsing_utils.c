/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 10:27:32 by haouky            #+#    #+#             */
/*   Updated: 2024/08/30 18:25:53 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char *get_path(char *s, t_list *env)
{
	char *path;
	char *p;
	char **paths;
	int i;

	i = 0;
	if(!s)
		return (0);
	if (access(s, X_OK) == 0 || find_c(s, '/'))
			return (str_dup(s, str_len(s, '\0')));
	p = envv("$PATH",env, 0);
	paths = ft_split(p, ':');
	free (p);
	while (paths && paths[i])
	{
		p = str_join(paths[i], "/");
		path = str_join(p, s);
		free(p);
		if (access(path, X_OK) == 0)
		{
			fr_double(paths);
			return (path);
		}
		free(path);
		i++;
	}
	fr_double(paths);
	return (NULL);
}

char *envv(char *lxr, t_list *env, int status)
{
	int j;
    char *s;
	
	while (env)
	{
		j = 0;
		if(lxr[j + 1] == '?')
		{
			s = ft_itoa(status);
			return (s);
		}
		while (env->str[j] && lxr[j + 1] && lxr[j + 1] == env->str[j])
			j++;
		if(!lxr[j + 1] && env->str[j] == '=' && env->str[j + 1])
		{
	        s = str_dup(&env->str[j + 1], str_len(&env->str[j + 1], 0));
			return (s);
		}
		env = env->next;
    }
    return (NULL);
}

char *eenvv(char *vvalue,char *prev, t_list **head, int what)
{
	char *tmp;
	char **dp;
	int i;
	int size;
	
	i = 0;
	size = 0;
	if(!vvalue)
	{
		if(what)
			return (str_dup("", 0));
		return (vvalue);
	}
	dp = ft_split(vvalue, ' ');
	while (dp[size])
		size++;
	free(vvalue);
	if(size > 1 && what)
	{
		fr_double(dp);
		return (NULL);
	}
	tmp = str_join(prev,dp[i++]);
	if(size > 1)
		add_back_lst(head, lst_new(tmp));
	else
	{
		fr_double(dp);
		return (tmp);
	}
	while (i < size - 1)	
		add_back_lst(head, lst_new(dp[i++]));
	if(dp[i])
		tmp = dp[i];
	free(dp);
	return (tmp);
}
t_lexer_list  *fqouts(t_list **head,t_lexer_list *lxr, t_list *env, int status)
{
	char *s;
	char *tmp;
	char *tmp1;
	enum e_token type;

	s = NULL;
	while (lxr && ((lxr->state != GENERAL) || (lxr->type != ' ' && lxr->type != '|' && lxr->type != '<' && lxr->type != '>' && lxr->type != HERE_DOC && lxr->type != DREDIR_OUT)))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
            if(lxr->type == ENV && lxr->state == GENERAL && lxr->len != 1)
				s = eenvv(envv(lxr->content, env, status) , s, head, 0);
            else if(lxr->type == ENV && lxr->state == IN_DQUOTE && lxr->len != 1)
			{
				tmp1 = envv(lxr->content, env, status);
				s = str_join(s, tmp1);
				free (tmp1);
			}
            else 
			    s = str_join(s,lxr->content);
			free(tmp);
		}
		type = lxr->type;
		lxr = lxr->next;
	}
	if(!s && type != ENV)
		s = str_dup("",0);
	if(s)
		add_back_lst(head, lst_new(s));
	return (lxr);
}

t_lexer_list  *ftqouts(t_oip **head,t_lexer_list *lxr, t_stat *stat, t_list  *env)
{
	char *s;
	char *tmp;
	char *tmp1;
	t_oip *node;
	enum e_state st;
	
	s = NULL;
	while (lxr && ((lxr->state != GENERAL) || (lxr->type != ' ' && lxr->type != '|' && lxr->type != '<' && lxr->type != '>' && lxr->type != HERE_DOC && lxr->type != DREDIR_OUT)))
	{
		if((lxr->type != QOUTE && lxr->type != DOUBLE_QUOTE ) || lxr->state != GENERAL)
		{
			tmp = s;
			if(lxr->type == ENV && lxr->state != IN_QUOTE && stat->type != HERE_DOC && lxr->len != 1)
			{
				tmp1 = envv(lxr->content, env, stat->exstat);
				if(lxr->state == GENERAL)
				{
                	tmp1 = eenvv(tmp1 , s, 0, 1);
					if(!tmp1)
					{
						stat->type = WORD;
						s = str_join(s, lxr->content);
					}
					else 
					{
						s = str_join(s, tmp1);
						free (tmp1);
					}
				}
				else
				{
					s = str_join(s, tmp1);
					free(tmp1);
				}
			}
            else 
			    s = str_join(s,lxr->content);
			free(tmp);
			st = lxr->state;
		}
		lxr = lxr->next;
	}
	node = flst_new(s);
	node->type = stat->type;
	if(stat->type == HERE_DOC && st != GENERAL)
		node->s = str_dup("", 0); 
	fadd_back_lst(head, node);
	return (lxr);
}
