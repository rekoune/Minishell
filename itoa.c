/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   itoa.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arekoune <arekoune@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/23 18:05:09 by haouky            #+#    #+#             */
/*   Updated: 2024/08/28 18:13:42 by arekoune         ###   ########.fr       */
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

int	find_c(char *s, char c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

long	ft_atoi(char *str)
{
	int		i;
	long	nb;
	int		sign;

	i = 0;
	sign = 1;
	nb = 0;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", str);
			exit(255);
		}
		nb = nb * 10;
		nb = nb + (str[i++] - 48);
	}
	nb *= sign;
	return (nb);
}
