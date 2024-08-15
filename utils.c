/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 08:52:12 by haouky            #+#    #+#             */
/*   Updated: 2024/08/15 11:22:10 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**getarray(t_list *lst)
{
	char	**str;
	int		i;

	i = 0;
	str = malloc((ft_lstsize(lst) + 1) * sizeof(char *));
	if (!str)
		return (0);
	while (lst)
	{
		str[i] = lst->str;
		i++;
		lst = lst->next;
	}
	str[i] = 0;
	return (str);
}

char	*str_join(char *s1, char *s2)
{
	char	*s;
	int		i;
	int		l;

	i = 0;
	l = 0;
	s = malloc(str_len(s1, '\0') + str_len(s2, '\0') + 1);
	while (s1 && s1[i])
	{
		s[i] = s1[i];
		i++;
	}
	while (s2 && s2[l])
	{
		s[i + l] = s2[l];
		l++;
	}
	s[i + l] = 0;
	return (s);
}
char	*str_dup(char *str, int size)
{
	int		i;
	char	*return_str;

	i = 0;
	return_str = malloc(sizeof(char) * (size + 1));
	if (!return_str)
		return (NULL);
	if(!str)
		return (0);
	while (i < size && str[i])
	{
		return_str[i] = str[i];
		i++;
	}
	return_str[i] = '\0';
	return (return_str);
}
static char	*sub(char const *s, unsigned int st, size_t l)
{
	size_t	i;
	char	*c;

	i = 0;
	while (s[st + i] != 0 && i < l)
		i++;
	c = malloc (i + 1);
	if (c == 0)
		return (0);
	i = 0;
	while (s[st + i] != 0 && i < l)
	{
		c[i] = s[st + i];
		i++;
	}
	c[i] = '\0';
	return (c);
}

char	*ft_substr(char *s, int st, size_t l)
{
	char	*c;

	if (!s)
		return (0);
	if (str_len(s, 0) < st)
	{
		c = malloc(1);
		if (c == 0)
			return (0);
		c[0] = '\0';
		return (c);
	}
	return (sub(s, st, l));
}