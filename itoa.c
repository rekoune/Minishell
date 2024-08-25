/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: haouky <haouky@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:05:09 by haouky            #+#    #+#             */
/*   Updated: 2024/08/25 12:15:56 by haouky           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*rev(char *c)
{
	int		i;
	int		l;
	char	*v;

	i = 0;
	l = strr_len(c) - 1;
	v = malloc (l + 2);
	if (v == 0)
		return (0);
	while (i <= l)
	{
		v[i] = c[l - i];
		i++;
	}
	v[i] = '\0';
	return (v);
}

char	*ft_itoa(int nb)
{
	long	n;
	char	c[12];
	int		i;
	char	*p;

	n = nb;
	i = 0;
	if (n < 0)
		n = -n;
	if (n == 0)
		c[i++] = '0';
	while (n > 0)
	{
		c[i] = n % 10 + 48;
		n = n / 10;
		i++;
	}
	if (nb < 0)
		c[i++] = '-';
	c[i] = '\0';
	p = rev(c);
	if (p == 0)
		return (0);
	return (p);
}

void	fr_double(char **s)
{
	int	i;

	i = 0;
	while (s && s[i])
		free(s[i++]);
	free(s);
}
