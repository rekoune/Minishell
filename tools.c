/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 10:15:02 by haouky            #+#    #+#             */
/*   Updated: 2024/08/11 16:05:29 by arekoune         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "minishell.h"

char	*str_ncopy(char *str, int size)
{
	int	i;
	char *s;
	
	i = 0;
	s = malloc((size + 1) * sizeof(char));
	if (!s)
		return (NULL);
	while(i < size && str[i])
	{
		s[i] = str[i];
		i++;
	}
	s[i] = '\0';
	return (s);
}

int	check_char(char c)
{
	if (c == ' ' || c == '<' || c == '>' || c == '\\' || c == '\'' || c == '\"' || c == '|' || c == '\n')	
		return(1);
	return (0);
}

int	str_comp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			return(s1[i] - s2[i]);
		i++;
	}
	return(s1[i] - s2[i]);
}

void	error(char *str)
{
	printf("%s", str);
	exit(1);
}

int	str_len(char *str, char separator)
{
	int	i;

	i = 0;
	if (!str)
		return(0);
	while (str[i] && str[i] != separator)
		i++;
	return (i);
}