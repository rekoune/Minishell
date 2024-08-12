/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 09:43:18 by haouky            #+#    #+#             */
/*   Updated: 2024/08/12 09:22:21 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_len(char *str, char separator)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	while (str[i] && str[i] != separator)
		i++;
	return (i);
}


int	word_counter(char *str, char separator)
{
	int	i;
	int	counter;
	int	flag;

	flag = 1;
	counter = 0;
	i = 0;
	while (str[i])
	{
		if (flag == 1 && str[i] != separator && str[i] != ' ')
		{
			flag = 0;
			counter++;
		}
		if (str[i] == separator)
			flag = 1;
		i++;
	}
	return (counter);
}

char	**ft_split(char *str, char separator)
{
	char	**return_str;
	int		counter;
	int		i;

	i = 0;
	counter = word_counter(str, separator);
	return_str = malloc(sizeof(char *) * (counter + 1));
	if (!return_str)
		return (NULL);
	while (i < counter)
	{
		while (*str == separator || *str == ' ')
			str++;
		return_str[i] = str_dup(str, str_len(str, separator));
		str += str_len(str, separator);
		i++;
	}
	return_str[i] = NULL;
	return (return_str);
}

