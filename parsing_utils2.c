/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 16:55:24 by haouky            #+#    #+#             */
/*   Updated: 2024/08/31 19:14:22 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_varibl(char *lxr, t_list *env, int status)
{
	int		j;
	char	*s;

	while (env)
	{
		j = 0;
		if (lxr[j + 1] == '?')
		{
			s = ft_itoa(status);
			return (s);
		}
		while (env->str[j] && lxr[j + 1] && lxr[j + 1] == env->str[j])
			j++;
		if (!lxr[j + 1] && env->str[j] == '=' && env->str[j + 1])
		{
			s = str_dup(&env->str[j + 1], str_len(&env->str[j + 1], 0));
			return (s);
		}
		env = env->next;
	}
	return (NULL);
}

char	*env4_arg(char *vvalue, char *prev, t_list **head)
{
	char	*tmp;
	char	**dp;
	int		i;
	int		size;

	i = 0;
	size = 0;
	dp = ft_split(vvalue, ' ');
	while (dp && dp[size])
		size++;
	free(vvalue);
	if (!dp || !dp[0])
	{
		free(dp);
		return (str_dup(prev, strr_len(prev)));
	}
	tmp = str_join(prev, dp[i]);
	add_back_lst(head, lst_new(tmp));
	while (++i < size - 1)
		add_back_lst(head, lst_new(str_dup(dp[i], strr_len(dp[i]))));
	tmp = str_dup(dp[i], strr_len(dp[i]));
	fr_double(dp);
	return (tmp);
}

char	*env_for_red(char *vvalue, char *prev, int *check, t_lexer_list *lxr)
{
	char	**dp;
	int		i;
	int		size;

	i = 0;
	size = 0;
	dp = ft_split(vvalue, ' ');
	while (dp && dp[size])
		size++;
	fr_double(dp);
	if (size > 1)
	{
		free(vvalue);
		*check = -1000;
		return (NULL);
	}
	else if (size < 1 && (!prev && (!lxr || (lxr && (lxr->type != -1
						&& lxr->type != '\"' && lxr->type != '\''
						&& lxr->type != '$')))))
	{
		free(vvalue);
		*check = -1000;
		return (NULL);
	}
	return (vvalue);
}
